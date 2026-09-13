#!/usr/bin/env python3
"""Inventory preserved inputs and generate a non-overlapping NE coverage ledger.
Manual review state lives separately in tracking/reviews.json and is never overwritten.
Run with --check to verify retained artifacts without changing them.
"""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parents[1]

def digest(data):
    return hashlib.sha256(data).hexdigest()

def read(path):
    return json.loads((ROOT/path).read_text())

def inventory():
    files, archives = [], []
    for p in sorted((ROOT/'target').rglob('*')):
        if not p.is_file():
            continue
        b=p.read_bytes()
        files.append(dict(path=p.relative_to(ROOT).as_posix(),size=len(b),sha256=digest(b),
                          role='macOS metadata' if p.name=='.DS_Store' else 'supplied input'))
        if zipfile.is_zipfile(p):
            with zipfile.ZipFile(p) as z:
                for i,member in enumerate(z.infolist()):
                    data=z.read(member)
                    archives.append(dict(archive=p.relative_to(ROOT).as_posix(),index=i,
                        member=member.filename,size=len(data),sha256=digest(data),
                        crc32=f'{member.CRC:08x}',zip_timestamp=list(member.date_time)))
    for member in archives:
        member['matching_files']=[f['path'] for f in files if f['sha256']==member['sha256']]
    return dict(schema_version=1,files=files,archive_members=archives,
                timestamp_note='ZIP timestamps are recorded metadata without timezone; they are not verified build dates.',
                version_evidence=dict(release_notes='26JFIN.txt describes V2.60j final release candidate; dated April 26, 2000.',
                    archive_label='stars26jrc3.zip',source_tree='c:\\user\\jeffl\\2.7j\\',
                    compiler='8.00c in all 40 extracted game-module compiler records',
                    user_clarification=dict(date='2026-09-13', version='2.70j RC3', edition='retail',
                        context='User identifies this as the third release candidate of the 2.70j patch. 2.60 was shareware; 2.70 was retail; the releases were binary compatible.'),
                    conclusion='Identified by the user as 2.70j RC3 (retail). Preserve archive/notes labels as provenance and SHA-256 as exact binary identity; product version has not been independently decoded from the executable.'))

def coverage():
    segments=read('analysis/segments.json')
    functions=read('analysis/functions.json')
    modules=read('analysis/modules.json')
    reviews=read('tracking/reviews.json')
    rows=[]
    for seg in segments:
        end=max(seg['size'],seg['allocation'])
        fs=[f for f in functions if f['ne_segment']==seg['segment']]
        points={0,end,seg['size']}
        for f in fs:
            points.update((f['ne_offset'],f['ne_offset']+f['size']))
        points=sorted(points)
        for start,stop in zip(points,points[1:]):
            owners=[f for f in fs if f['ne_offset']<=start and stop<=f['ne_offset']+f['size']]
            key=f"{seg['segment']:04x}:{start:04x}-{stop:04x}"
            file_backed=seg['file_offset'] is not None and stop<=seg['size']
            row=dict(id=key,ne_segment=seg['segment'],start=start,end_exclusive=stop,bytes=stop-start,
                     classification='debug_procedure_range' if owners else ('data_unreviewed' if seg['flags']&1 else 'unclassified_code_segment_range'),
                     storage='file' if file_backed else 'zero_fill',
                     file_offset=seg['file_offset']+start if file_backed else None,
                     names=[f['name'] for f in owners],modules=sorted(set(modules[str(f['module'])]['name'] for f in owners)),
                     evidence=['analysis/segments.json']+(['analysis/functions.json','analysis/segment_map.json'] if owners else []),
                     reconstruction_path=None,review_status='not_started',validation_status='not_started')
            if key in reviews:
                row.update(reviews[key])
            rows.append(row)
    unknown=set(reviews)-{r['id'] for r in rows}
    if unknown:
        raise ValueError(f'Review keys no longer map to coverage: {unknown}')
    return dict(schema_version=1,address_convention='NE ordinal:offset; half-open ranges; not runtime selectors',
        scope='NE segment static memory extents only. DOS stub, headers, relocations, resources and debug overlay are outside this denominator and remain step 1 work.',
        classification_note='Procedure ranges are debug evidence, not proof that every byte is an instruction. Gaps are not assumed to be padding or library code.',
        summary=dict(segments=len(segments),ranges=len(rows),segment_bytes=sum(r['bytes'] for r in rows),
            debug_procedure_bytes=sum(r['bytes'] for r in rows if r['names']),
            unclassified_code_segment_bytes=sum(r['bytes'] for r in rows if r['classification']=='unclassified_code_segment_range'),
            data_segment_bytes=sum(r['bytes'] for r in rows if r['classification']=='data_unreviewed'),
            compiled_reconstruction_bytes=0,
            manually_reviewed_bytes=sum(r['bytes'] for r in rows if r['review_status']=='manually_reviewed')),
        ranges=rows)

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--check',action='store_true')
    args=ap.parse_args()
    for filename,obj in [('analysis/input_inventory.json',inventory()),('analysis/coverage.json',coverage())]:
        content=json.dumps(obj,indent=2)+'\n'
        path=ROOT/filename
        if args.check:
            if path.read_text()!=content:
                raise SystemExit('Baseline differs: '+filename)
            print('Verified '+filename)
        else:
            path.write_text(content)
            print('Generated '+filename)

if __name__=='__main__':
    main()
