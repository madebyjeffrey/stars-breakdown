#!/usr/bin/env python3
"""Install the pinned, isolated macOS arm64 analysis tools; no system installation."""
import hashlib
import os
from pathlib import Path
import platform
import shutil
import subprocess
import urllib.request
import zipfile

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / '.tools'
GHIDRA = TOOLS / 'ghidra_12.1.3_PUBLIC'
JDK = TOOLS / 'jdk-21.0.12.1+1/Contents/Home'
PACKAGES = [
    ('ghidra.zip', 'https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_12.1.3_build/ghidra_12.1.3_PUBLIC_20260817.zip', '93a5d11a9ad510622acaaf908c556a7b9b764d338e78a7567f3689bf5081fd54'),
    ('jdk.tar.gz', 'https://github.com/adoptium/temurin21-binaries/releases/download/jdk-21.0.12.1%2B1/OpenJDK21U-jdk_aarch64_mac_hotspot_21.0.12.1_1.tar.gz', '3623232f33a9c3baadf304480b2535f9a3cba8a58d42ecbb438ba267315d9998'),
]

def main():
    if (platform.system(), platform.machine()) != ('Darwin', 'arm64'):
        raise SystemExit('This setup is tested on macOS arm64 only. Select and verify matching packages before porting.')
    downloads = TOOLS / 'downloads'
    downloads.mkdir(parents=True, exist_ok=True)
    for name, url, digest in PACKAGES:
        target = downloads / name
        # Reuse an already verified download even if originally saved under another filename.
        if not target.exists():
            for candidate in downloads.iterdir():
                if candidate.is_file() and candidate.stat().st_size > 100_000_000 and hashlib.file_digest(candidate.open('rb'), 'sha256').hexdigest() == digest:
                    target = candidate
                    break
        if not target.exists():
            print('Downloading', name, flush=True)
            urllib.request.urlretrieve(url, target)
        with target.open('rb') as stream:
            assert hashlib.file_digest(stream, 'sha256').hexdigest() == digest, 'Download checksum mismatch'
        if name == 'ghidra.zip' and not GHIDRA.exists():
            with zipfile.ZipFile(target) as archive:
                archive.extractall(TOOLS)
        if name == 'jdk.tar.gz' and not JDK.exists():
            subprocess.run(['tar', '-xzf', str(target), '-C', str(TOOLS)], check=True)
    for name in ('launch.sh', 'analyzeHeadless', 'ghidraRun'):
        path = GHIDRA / ('support/' + name if name != 'ghidraRun' else name)
        path.chmod(path.stat().st_mode | 0o111)
    subprocess.run(['python3', str(ROOT / 'tools/patch_ghidra.py')], check=True)
    cpp = GHIDRA / 'Ghidra/Features/Decompiler/src/decompile/cpp'
    subprocess.run(['make', '-j4', 'ghidra_opt', 'ARCH_TYPE=-arch arm64', 'OSDIR=mac_arm_64'], cwd=cpp, check=True)
    destination = GHIDRA / 'Ghidra/Features/Decompiler/os/mac_arm_64/decompile'
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(cpp / 'ghidra_opt', destination)
    (TOOLS / 'user').mkdir(exist_ok=True)
    print('Isolated decompiler ready:', GHIDRA)

if __name__ == '__main__':
    main()
