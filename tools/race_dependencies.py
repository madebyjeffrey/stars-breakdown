#!/usr/bin/env python3
"""Audit the race component's direct calls, modal callbacks and review evidence."""
import argparse
import json
from collections import defaultdict
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
BINDINGS={}
for source,pairs in {
    'runtime.c':'FSaveRace:save_race FWasRaceFile:was_race_file',
    'password.c':'Delay:delay FCheckPassword:check_password LSaltFromSz:password_salt PasswordDlg:password_dialog',
    'output.c':'OutputFileString:output_file_string OutputSz:output_string SzVersion:version_string',
    'format.c':'PszFormatString:format_string PszFormatIds:format_id PszCalcEnvVar:environment_string PszCalcGravity:gravity_string',
    'ui.c':'CtrTextOut:center_text RcCtrTextOut:rect_center_text RightTextOut:right_text StickyDlgPos:sticky_dialog '
        '_Draw3dFrame:draw_frame InitBtnTrack:init_button_track FTrackBtn:track_button DrawBtn:draw_button '
        'FGetMouseMove:get_mouse_move DibNumColors:dib_num_colors PaletteSize:palette_size DibBlt:dib_blt',
    'raceui.c':'InvalidateAdvPtsRect:invalidate_advantage DrawRaceAdvantagePoints:draw_advantage SetRCWTitle:race_title',
    'racewizard.c':'RaceCreationWizard:race_wizard '+ ' '.join('RaceWizardDlg%d:race_dialog%d'%(i,i) for i in range(1,7)),
    'racedraw.c':'DrawRace2:draw_race2 DrawRace3:draw_race3 IrcRaceDlgHitTest:race_hit_test FTrackRaceDlg2:track_race2 FTrackRaceDlg3:track_race3',
}.items():
    for pair in pairs.split():
        original,host=pair.split(':');BINDINGS[original]={'source':'reconstructed/src/'+source,'host_entry':'stars_'+host}

SCOPED={
    **{n:'Private name-lookup implementation inside format.c; validated through PszFormatString, not credited as a separately reconstructed entry point.'
       for n in ['PszGetCompressedPlanet','LpthFromId','LpflFromId','PszGetThingName','PszGetFleetName','PszFleetNameFromWord','PszGetPlanetName']},
    'PszGetLocName':'Only the grobj=0, id=-1 coordinate-formatting path is required by this caller.',
    'IshdefPrimaryFromLpfl':'Only the non-null pcDiff path is required by fleet_name; valid owner/design storage required.',
    **{n:'The formatter observes only the selected component pointer. part_name implements that data path; availability results are discarded and queries do not mutate game globals. All valid table entries compared while the original executes these queries.'
       for n in ['FLookupPart','FShouldPartBeHidden','TechStatus']},
}
ROOTS=['FSaveRace','FWasRaceFile','RaceCreationWizard','CreateRandomRace','PasswordDlg']+['RaceWizardDlg%d'%i for i in range(1,7)]
def read(path):return json.loads((ROOT/path).read_text())
def write(path,value): (ROOT/path).write_text(json.dumps(value,indent=2)+'\n')

def update_reviews():
    reviews=read('tracking/reviews.json');procedures={p['name']:p for p in read('analysis/debug/procedures.json')}
    for name,binding in BINDINGS.items():
        p=procedures[name];start=p['ne_offset'];end=start+p['size'];key=f"{p['ne_segment']:04x}:{start:04x}-{end:04x}"
        reviews[key]={
            'reconstruction_path':binding['source'],'review_status':'manually_reviewed',
            'compilation_status':'host_c89_compiled',
            'validation_status':'original_x86_differential_checked; integrated_race_component; synthetic_Windows_services; native_Win16_integration_pending',
            'evidence':['analysis/functions.json','analysis/debug/procedures.json','analysis/decompiler/export-manifest.json',
                        'analysis/reconstruction/race-component.md','analysis/reconstruction/message-formatting.md',
                        'tests/test_race_runtime.py','tests/test_race_format.py','tests/test_race_ui.py']}
    write('tracking/reviews.json',reviews)

def audit():
    functions=read('analysis/decompiler/functions.json');functions_by_name={f['name']:f for f in functions}
    functions_by_address={f['address']:f for f in functions}
    procedures={f"{0x1000+(p['ne_segment']-1)*8:04x}:{p['ne_offset']:04x}":p
                for p in read('analysis/debug/procedures.json') if p.get('ne_segment')}
    reviewed={n:r for r in read('analysis/coverage.json')['ranges'] if r['review_status']=='manually_reviewed' for n in r['names']}
    edges=defaultdict(list)
    for e in read('analysis/decompiler/callgraph.json'):edges[e['caller']].append(e)
    queue=[functions_by_name[n]['address'] for n in ROOTS];seen=set();external=set()
    while queue:
        address=queue.pop()
        if int(address.split(':')[0],16)>=0x1118:external.add(address);continue
        if address in seen:continue
        seen.add(address)
        queue.extend(e['target'] for e in edges[address])
    rows=[];unaccounted=[]
    for address in sorted(seen):
        p=procedures.get(address);name=p['name'] if p else functions_by_address.get(address,{}).get('name',address)
        row={'name':name,'analysis_address':address}
        if p:row.update(ne_segment=p['ne_segment'],offset=p['ne_offset'],end_exclusive=p['ne_offset']+p['size'],procedure_bytes=p['size'])
        if name in BINDINGS:row.update(BINDINGS[name],status='reconstructed_entry')
        elif name in reviewed:row.update(status='previously_reconstructed',source=reviewed[name]['reconstruction_path'])
        elif name in SCOPED:row.update(status='required_call_path_implemented',source='reconstructed/src/format.c',scope=SCOPED[name])
        else:row['status']='unaccounted';unaccounted.append(name)
        rows.append(row)
    assert not unaccounted,('Unaccounted dependencies',unaccounted)
    resources=[]
    for resource in read('analysis/ne/layout.json')['resources']:
        if (resource['type']==5 and resource['id'] in [0x92,0x93,0x94,0x95,0x96,0x97,0x8c]) or (resource['type']==2 and resource['id']==0x85):
            resources.append({k:resource[k] for k in ['type','id','bytes','sha256','extracted_path']})
    callbacks=[{'resource':res,'procedure':name} for res,name in zip([0x92,0x95,0x96,0x93,0x94,0x97,0x8c],
        ['RaceWizardDlg1','RaceWizardDlg4','RaceWizardDlg5','RaceWizardDlg2','RaceWizardDlg3','RaceWizardDlg6','PasswordDlg'])]
    return dict(schema_version=1,scope='Race save/load, six-page wizard and deferred random-race generation; OS/runtime contracts are explicit.',
        roots=ROOTS,callbacks=callbacks,dependencies=rows,
        runtime_and_import_targets=[{'analysis_address':a,'name':functions_by_address.get(a,{}).get('name',a)} for a in sorted(external)],
        resources=resources,
        entry_slice={'name':'CommandHandler(hwnd, 0x81)','ne_segment':5,'offset':0x3243,'end_exclusive':0x3269,
                     'host_entry':'stars_new_race','scope':'Copy the default 192-byte player then call the wizard. Other menu actions and startup are outside this component.'},
        summary={'accounted_game_targets':len(rows),'unaccounted_game_targets':len(unaccounted),
                 'new_reviewed_entries':len(BINDINGS),'new_reviewed_procedure_bytes':sum(p['size'] for p in procedures.values() if p['name'] in BINDINGS),
                 'scoped_helper_paths':sum(r['status']=='required_call_path_implemented' for r in rows),
                 'native_windows_acceptance':'pending; no new VM test package prepared'})

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--check',action='store_true');parser.add_argument('--update-reviews',action='store_true')
    args=parser.parse_args()
    if args.update_reviews:update_reviews();return
    value=audit();path='analysis/reconstruction/race-dependencies.json'
    if args.check:assert read(path)==value,'Race dependency audit differs'
    else:write(path,value)
    print(json.dumps(value['summary'],indent=2))

if __name__=='__main__':main()
