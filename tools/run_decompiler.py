#!/usr/bin/env python3
"""Create a fresh Ghidra project and regenerate the step-3 evidence."""
import argparse
import os
from pathlib import Path
import subprocess
from datetime import datetime
from setup_decompiler import ROOT, GHIDRA, JDK


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project', default='stars-' + datetime.now().strftime('%Y%m%d-%H%M%S'))
    args = parser.parse_args()
    if not (GHIDRA / 'support/analyzeHeadless').exists():
        raise SystemExit('Run python3 tools/setup_decompiler.py first.')
    out = ROOT / 'analysis/decompiler/export'
    if out.exists():
        # Keep previous exports; never accidentally mix output from separate runs.
        saved = out.with_name('export-' + datetime.now().strftime('%Y%m%d-%H%M%S'))
        out.rename(saved)
    out.mkdir(parents=True)
    env = os.environ.copy()
    env['JAVA_HOME'] = str(JDK)
    env['JAVA_TOOL_OPTIONS'] = '-Duser.home=' + str(ROOT / '.tools/user')
    command = [str(GHIDRA / 'support/analyzeHeadless'), str(ROOT / 'ghidra_projects'), args.project,
               '-import', str(ROOT / 'target/stars.exe'), '-processor', 'x86:LE:16:Protected Mode',
               '-scriptPath', str(ROOT / 'tools/ghidra'), '-preScript', 'ImportStars.java', str(ROOT),
               '-postScript', 'ExportStars.java', str(out), '-postScript', 'AuditStars.java', str(out),
               '-analysisTimeoutPerFile', '600']
    with (out / 'headless.log').open('w') as log:
        subprocess.run(command, cwd=ROOT, env=env, stdout=log, stderr=subprocess.STDOUT, check=True)
    stub=ROOT/'.tools/dos-stub.bin'
    stub.write_bytes((ROOT/'target/stars.exe').read_bytes()[0x200:0x262])
    dos_command=[str(GHIDRA/'support/analyzeHeadless'),str(ROOT/'ghidra_projects'),args.project,
                 '-import',str(stub),'-loader','BinaryLoader','-loader-baseAddr','1000:0000',
                 '-processor','x86:LE:16:Real Mode','-noanalysis','-scriptPath',str(ROOT/'tools/ghidra'),
                 '-postScript','ImportDosStub.java','-postScript','ExportStars.java',str(out/'dos')]
    with (out/'dos-headless.log').open('w') as log:
        subprocess.run(dos_command,cwd=ROOT,env=env,stdout=log,stderr=subprocess.STDOUT,check=True)
    # Ghidra sometimes returns zero after a script failure: require the actual products.
    for filename in ('functions.json', 'units.json', 'invalid-flows.json'):
        if not (out / filename).exists():
            raise SystemExit('Missing ' + filename + '; inspect ' + str(out / 'headless.log'))
    subprocess.run(['python3', str(ROOT / 'tools/decompiler_report.py')], cwd=ROOT, check=True)
    print('Project:', ROOT / 'ghidra_projects' / args.project)
    print('Report:', ROOT / 'analysis/decompiler/README.md')

if __name__ == '__main__':
    main()
