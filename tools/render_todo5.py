#!/usr/bin/env python3
"""Render the detailed Step 5 inventory from checked-in analysis (no binary access)."""
from pathlib import Path
import collections
import json
import re

ROOT = Path(__file__).resolve().parents[1]
def read(name):
    return json.loads((ROOT / name).read_text())
def link(path, label=None):
    return f'[{label or Path(path).name}]({path})'
def key(address):
    seg, off = (int(x, 16) for x in address.split(':'))
    assert seg >= 0x1000 and (seg - 0x1000) % 8 == 0
    return (seg - 0x1000) // 8 + 1, off
def anchor(k):
    return f'fn-{k[0]:04x}-{k[1]:04x}'

# Grouping is an editorial navigation aid; original module identity is retained.
GROUPS = [
 ('startup', 'Application startup, configuration and main frame', ['stars','init','mdi'], 'Finish original WinMain, initialization order, command-line/batch modes, main-frame dispatch, host mode and every INI path. The new START application bootstrap is integration scaffolding, not completion of these original functions.'),
 ('common', 'Memory, random numbers, formatting and shared services', ['memory','utilgen','util','strings'], 'Preserve near/far pointer behavior, heap ownership, aliasing, 16/32-bit wrapping, RNG consumption and shared scratch buffers. Complete lookup/duplication paths and shared drawing, input, geometry and password services.'),
 ('components', 'Component definitions and world objects', ['parts','thing'], 'Recover all component selection/availability rules and table identities, object allocation/lifetime and thing interactions. Test valid and missing IDs and exact state changes.'),
 ('persistence', 'Files, serialization, orders and logs', ['file','save','log'], 'Extend the race-file component to complete game/turn/history/log persistence. Compare exact records, encryption/checksums, version gates, error/retry behavior, replay ordering and cleanup.'),
 ('creation', 'Race creation, new games and ship design', ['race','create','build'], 'Complete world generation, all new-game wizard paths and ship builder behavior alongside the reviewed race component. Compare seeds, all generated state, component restrictions, queue edits and cancellation.'),
 ('simulation', 'Planets, fleets, production, research and turn processing', ['planet','ship','ship2','mine','produce','research','turn','turn2','turn3'], 'Recover exact phase ordering and state mutations: movement, fuel, population, production, minerals, research, terraforming and events. These modules also contain UI: preserve their message/drawing paths rather than treating the category as simulation-only.'),
 ('combat', 'Combat resolution and battle playback', ['battle','vcr'], 'Compare token initialization, initiative/ties, movement, target choice, damage, shields, casualties, salvage, bombing and RNG order; verify battle records and playback against the same resolved battle.'),
 ('ai', 'AI personalities and shared decision machinery', ['ai','ai2','ai3','ai4','aiutil'], 'Recover each decision rule and personality path, including design selection, production, target selection, logistics and fallback choices. Compare entire orders/state plus RNG draws for controlled worlds and ties.'),
 ('interface', 'Scanner, toolbar, messages, reports and popups', ['scan','tb','msg','report','popup'], 'Map every window/dialog message and command, hit test, coordinate transform, selection, sort/filter, drawing path and UI state transition. Test resource IDs, keyboard/mouse routes, resize/repaint and cancellation.'),
 ('tutorial', 'Tutorial progression and restrictions', ['tutor','tutor2'], 'Recover tutorial task gates, prompts, state save/restore, restrictions and all success/error/abort transitions. Compare progression against controlled tutorial fixtures.'),
]
PARTIAL = {
 'FCreateStuff': ('reconstructed/src/startup.c', 'Only bounded startup integrations, including six planet-attribute brushes at NE 0001:023d–02af; complete resource/global initialization remains open.', 'analysis/compat/startup2a-native.json'),
 'ReadIniSettings': ('reconstructed/src/config.c', 'Frame/layout/pane/toolbar/tile/backup/font paths are implemented; remaining preferences and complete original control flow remain open.', 'analysis/reconstruction/start2.md'),
 'WriteIniSettings': ('reconstructed/src/config.c', 'Scoped geometry/layout/pane/toolbar persistence is implemented; complete original settings writer remains open.', 'analysis/reconstruction/start2.md'),
 'TitleWndProc': ('reconstructed/src/title.c', 'Reviewed title creation/painting/palette/destruction paths are integrated; complete original command/application behavior remains open. The trailing zero-fill byte belongs to this same procedure.', 'analysis/reconstruction/starting-screen.md'),
}

def main():
    ranges = read('analysis/coverage.json')['ranges']
    procedures = {(p['ne_segment'], p['ne_offset']): p for p in read('analysis/debug/procedures.json')}
    games = []
    for k, p in procedures.items():
        pieces = [r for r in ranges if r['classification']=='debug_procedure_range' and r['ne_segment']==k[0] and k[1]<=r['start']<k[1]+p['size']]
        assert sum(r['bytes'] for r in pieces)==p['size']
        assert len({r['review_status'] for r in pieces})==1
        r = dict(pieces[0]); r['end_exclusive']=k[1]+p['size'];r['bytes']=p['size']
        games.append(r)
    funcs = read('analysis/decompiler/functions.json')
    fs = {key(f['address']): f for f in funcs}
    gs = {(r['ne_segment'], r['start']): r for r in games}
    procedures = {(p['ne_segment'], p['ne_offset']): p for p in read('analysis/debug/procedures.json')}
    assert len(gs) == len(games) == len(procedures) == 846
    assert len(fs) == len(funcs) == 1043 and gs.keys() <= fs.keys()
    modules = {m + '.obj' for _, _, mods, _ in GROUPS for m in mods}
    assert modules == {m for r in games for m in r['modules']}
    assert all(len(r['modules']) == 1 for r in games)
    signatures = {}
    for line in (ROOT/'analysis/debug/SIGNATURES.md').read_text().splitlines():
        cols = [c.strip().strip('`') for c in line.split('|')[1:-1]]
        if len(cols) == 6 and re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}', cols[2]):
            signatures[tuple(int(c,16) for c in cols[2].split(':'))] = cols
    assert signatures.keys() == gs.keys()
    calls = collections.defaultdict(list)
    incoming = collections.defaultdict(set)
    for c in read('analysis/decompiler/callgraph.json'):
        calls[c['caller']].append(c)
        incoming[c['target']].add(c['caller'])
    indirect = collections.defaultdict(list)
    for x in read('analysis/decompiler/indirect-targets.json'):
        for owner in x['owners']:
            indirect[owner].append(x)
    data = collections.defaultdict(set)
    for d in read('analysis/decompiler/data-references.json'):
        for ref in d['references']:
            data[ref['function']].add('/'.join(d['names']) + '@' + d['address'])
    deps = {d['name']:d for d in read('analysis/reconstruction/race-dependencies.json')['dependencies'] if d['status']=='required_call_path_implemented'}
    runtime = read('analysis/reconstruction/runtime-library.json')['modules']
    exports = read('analysis/decompiler/export-manifest.json')
    raw = {x['path'].split('/')[-1].split('_',2)[0]+'_'+x['path'].split('/')[-1].split('_',2)[1]:x['path'] for x in exports if x['path'].startswith('raw/') and x['path'].endswith('.c')}
    reviewed = lambda r:r['review_status']=='manually_reviewed'
    partial = lambda r:not reviewed(r) and (r['names'][0] in PARTIAL or r['names'][0] in deps)
    done=sum(map(reviewed,games)); part=sum(map(partial,games))
    credited=sum(r['bytes'] for r in games if reviewed(r)); total=sum(r['bytes'] for r in games)
    out=[]
    def add(s=''):out.append(s)
    add('# TODO5 — complete function-by-function reconstruction checklist')
    add('\nInventory snapshot: 2026-09-14. This expands [Step 5 of TODO.md](TODO.md#5-reconstruct-the-program-subsystem-by-subsystem); it does not replace the overall project plan. Regenerate with `python3 tools/render_todo5.py` after updating the authoritative review records and analysis. Edit scope notes/grouping in that generator, not the generated rows. No original binary is read by this renderer.')
    add('\n## How to read this list')
    add('\nEvery currently identified function has its own entry. Game routines are grouped by their original object module, under editorial subsystem headings. A module may mix UI and rules. Identity is keyed by **NE segment and entry offset**, never by spelling alone. Three final-byte zero-fill ranges are joined to their procedures, giving 846 functions rather than 849 coverage ranges. Addresses here are hexadecimal NE ordinals, not runtime selectors. All end offsets are exclusive; raw Ghidra addresses use a separate synthetic selector space.')
    add('\n- **[x] Reviewed source** means the whole original game procedure has reviewed, compiled C and the ledger’s original-instruction validation. It does not mean the full application or every native caller is integrated.\n- **[ ] Partial** means a documented slice or caller-specific substitute exists; the whole procedure remains open.\n- **[ ] Open** means no whole-function source credit is recorded; raw decompilation already exists and is a starting point.\n- **[x] Library mapping** in the runtime appendix means the exported body lies within audited matching CRT code spans. It is not recovered runtime source, independent behavioral validation or final link integration.\n- DOS-stub completion is counted separately. Neither CRT entries nor DOS entries inflate game-source coverage.')
    add('\nThe source/evidence fields come from [coverage.json](analysis/coverage.json), maintained through [tracking/reviews.json](tracking/reviews.json). Some ledger validation strings still say native integration pending: recent package-level acceptance does not establish every function/caller combination. [START2A Windows 95 acceptance](analysis/compat/startup2a-win95.json) verifies the integrated title, wizard, colors and race roundtrip. It does not complete WinMain, world generation, game loading or turn processing.')
    add('\n## Counts and navigation')
    add(f'\n- **846 named game procedures:** {done} reviewed, {part} documented partial entries, {846-done-part} open.\n- **{credited:,} / {total:,} named procedure bytes** credited ({credited/total:.2%}); procedure extents can include embedded data. This is not whole-executable instruction coverage.\n- **197 additional NE runtime/helper entries**, each listed below; 101 code-bearing CRT module records are audited, a different denominator.\n- **2 DOS-stub entries**, separately listed.\n- **1,045 total function entries** in this document. This is complete for the current inventory, not proof that future boundary/dispatch analysis cannot discover more code.')
    add('\n| Subsystem | Named procedures | Reviewed | Partial | Open |')
    add('| --- | ---: | ---: | ---: | ---: |')
    for ident,title,mods,_ in GROUPS:
        rows=[r for r in games if r['modules'][0] in [m+'.obj' for m in mods]]
        n=sum(map(reviewed,rows));p=sum(map(partial,rows))
        add(f'| [{title}](#{ident}) | {len(rows)} | {n} | {p} | {len(rows)-n-p} |')
    add('\nAlso: [runtime/helper functions](#runtime), [DOS stub](#dos-stub), [non-function obligations](#non-functions), [completion rules](#completion-rules).')
    add('\n## Completion rules')
    add('\nFor each open/partial entry, finish all of these before checking its main box:')
    add('\n1. Confirm original boundaries, debug signature, near/far ABI, DS/SS/ES assumptions, locals and embedded tables against the disassembly.\n2. Resolve every basic block, direct/indirect call, callback, data reference and resource dependency. The calls listed below are the exported static graph, not a proof of all runtime dispatch.\n3. Write readable, compilable source preserving widths, signedness, carry/wrap, shifts, pointer aliasing, x87 precision/rounding, RNG order, side effects and error/early-exit paths.\n4. Compare normal and boundary/error cases against original instructions or documented native integration where appropriate. Check complete relevant state and service-call order, not just a return value.\n5. Compile with the component; record evidence and exact scope in the review ledger. Native application integration remains a separate milestone where only a host semantic adapter exists.')
    add('\n**Per-entry detail:** “Signature” is the recovered effective debug description, not a host replacement prototype; `…` denotes no parameters recorded in that metadata, not a claim of variadic arguments. “Calls” includes every edge in the current exported graph, with duplicate call sites collapsed. “Callers” is a static count. Data-reference counts/labels are tool output and need semantic review; false labels are possible. Indirect-site notes are scoped by address to avoid merging same-named procedures. Module acceptance focus describes planned checks, not claims that those checks have already passed.')
    emitted=[]
    def call_text(f):
        result=[]
        for c in calls[f['address']]:
            label=c['target_names'][0] if c['target_names'] else c['target']
            k=key(c['target']) if c['target'].split(':')[0] != 'EXTERNAL' else None
            if k in fs: result.append(f'[`{label}`](#{anchor(k)})')
            else: result.append(f'`{label}` ({c["target"]})')
        return (', '.join(result)+'.') if result else 'No outgoing edge recorded (not proof of no OS/indirect calls).'
    def rawlinks(f):
        prefix=f['address'].replace(':','_')
        asm='analysis/decompiler/export/raw/'+prefix+'.asm'
        c='analysis/decompiler/export/'+raw[prefix]
        assert (ROOT/asm).exists() and (ROOT/c).exists()
        return link(asm,'assembly')+' · '+link(c,'raw pseudocode')
    for ident,title,mods,focus in GROUPS:
        add(f'\n<a id="{ident}"></a>\n\n## {title}')
        add('\n**Subsystem exit / acceptance focus:** '+focus)
        for mod in mods:
            rows=sorted([r for r in games if r['modules']==[mod+'.obj']],key=lambda r:(r['ne_segment'],r['start']))
            add(f'\n### {mod}.obj — {len(rows)} procedures, {sum(map(reviewed,rows))} reviewed')
            for r in rows:
                k=(r['ne_segment'],r['start']);emitted.append(k);f=fs[k];name=r['names'][0];s=signatures[k]
                params=s[5] if s[5]!='(none recorded)' else '…'
                state='Reviewed source' if reviewed(r) else 'Partial' if partial(r) else 'Open'
                add(f'\n<a id="{anchor(k)}"></a>\n\n- [{"x" if reviewed(r) else " "}] **`{name}` — {state}**')
                add(f'  - Identity: NE `{k[0]:04x}:{k[1]:04x}–{r["end_exclusive"]:04x}`; **{r["bytes"]:,} bytes**; {rawlinks(f)}.')
                add(f'  - Signature: `{s[3]} {name}({params})`; **{s[4]}**. Debug source: '+', '.join('`'+x.split('\\')[-1]+'`' for x in procedures[k]['source_files'])+'.')
                add(f'  - Calls: {call_text(f)} Static callers: {len(incoming[f["address"]])}.')
                refs=sorted(data[f['address']]);sites=[x for x in indirect[name] if key(x['address'])[0]==k[0] and k[1]<=key(x['address'])[1]<r['end_exclusive']]
                if refs: add(f'  - Data references: {len(refs)} recorded targets; '+', '.join('`'+x+'`' for x in refs[:8])+(' … (remaining targets in [data-reference index](analysis/decompiler/data-references.json)).' if len(refs)>8 else '.'))
                if sites: add('  - Indirect dispatch to review: '+'; '.join('`'+x['address']+'` '+x['kind']+' — '+x['status']+f' ({len(x["targets"])} recorded targets)' for x in sites)+'. See [target sets](analysis/decompiler/indirect-targets.json).')
                if reviewed(r):
                    evidence=[e for e in r['evidence'] if e.startswith(('tests/','analysis/reconstruction/','analysis/compat/'))]
                    add('  - Implementation: '+link(r['reconstruction_path'])+'. Evidence: '+' · '.join(link(e) for e in evidence)+'.')
                    add('  - Recorded validation: `'+r['validation_status']+'`. Retain these comparisons when connecting additional callers; no further whole-function source credit is needed.')
                else:
                    if name in PARTIAL:
                        src,scope,ev=PARTIAL[name];add('  - Existing slice: '+link(src)+' — '+scope+' '+link(ev,'Scope evidence')+'.')
                    elif name in deps:
                        d=deps[name];add('  - Existing slice: '+link(d['source'])+' — '+d['scope'])
                    add('  - [ ] '+('Complete the remaining original body beyond the documented slice' if partial(r) else 'Reconstruct and review the complete original body')+'; resolve the calls/data/dispatch above using the completion rules.')
                    add('  - [ ] Compile and validate normal, boundary and failure paths against the original; apply this subsystem’s acceptance focus and attach evidence before promotion.')
    assert len(emitted)==846 and set(emitted)==set(gs)
    add('\n<a id="runtime"></a>\n\n## Runtime and compiler helpers — all 197 additional NE entries')
    add('\nThese are individually enumerated even when they share a CRT object member. Names beginning `FUN_` are generated labels. Signatures are raw tool hypotheses here, not recovered game-debug contracts. The membership check below requires every exported body range to fit audited code; matching code does not settle data slots, ABI use at every call site or final library/linker placement. See [runtime-library audit](analysis/reconstruction/runtime-library.json), [data audit](analysis/reconstruction/runtime-data.md) and [foundation report](analysis/reconstruction/foundations.md).')
    extras=sorted(set(fs)-set(gs)); mapped=0
    for k in extras:
        f=fs[k]; owners=[]; covered=True
        for ran in f['ranges']:
            a=key(ran['start']);b=key(ran['end'])
            candidates=[m for m in runtime if m.get('status')=='relocation_consistent_code_match' and m.get('ne_segment')==a[0]==b[0] and m['ne_offset']<=a[1] and b[1]<m['ne_offset']+m['code_bytes']]
            if not candidates:covered=False
            owners.extend(m['name'] for m in candidates)
        mapped+=covered
        add(f'\n<a id="{anchor(k)}"></a>\n\n- [{"x" if covered else " "}] **`{f["name"]}` — {"Library mapping" if covered else "Mapping review needed"}**')
        add(f'  - NE `{k[0]:04x}:{k[1]:04x}`; {f["body_bytes"]} exported body bytes; {rawlinks(f)}. Body ranges: '+', '.join(r['start']+'–'+r['end']+' inclusive' for r in f['ranges'])+'.')
        add(f'  - Tool signature: `{f["signature"]}`; `{f["convention"]}`; return storage `{f["return_storage"]}`.')
        add('  - Audited member(s): '+(', '.join('`'+x+'`' for x in sorted(set(owners))) or 'No fully containing audited code span')+'.')
        add('  - Calls: '+call_text(f)+f' Static callers: {len(incoming[f["address"]])}.')
        add('  - [ ] Close final integration: select the matching library implementation (or document a validated substitute), preserve calling/return and data contracts, and verify its actual callers in the final linked program. Do not count this as handwritten runtime source.')
    add(f'\n**Mapping cross-check:** {mapped} / {len(extras)} exported runtime/helper bodies fit audited matching CRT code spans. The remaining entries, if any, need boundary/mapping review even though module-level matches exist.')
    add('\n<a id="dos-stub"></a>\n\n## DOS stub — both entries')
    for f in read('analysis/decompiler/dos-functions.json'):
        add(f'\n- [x] **`{f["name"]}`** — separate DOS address `{f["address"]}`, {f["body_bytes"]} exported instruction bytes. Reconstructed in [dos_stub.s](reconstructed/src/dos_stub.s), validated by [test_dos_stub.py](tests/test_dos_stub.py). The whole 98-byte entry image matches; instruction-body counts exclude intervening header/message data.')
    add('\n<a id="non-functions"></a>\n\n## Work that a function list cannot mark complete')
    add('\nThese obligations prevent “all names checked” from being mistaken for a full decompilation.')
    add('\n- [ ] **Original startup and final linking:** integrate every required subsystem under the recovered initialization/message/termination paths, without delegating work to the original executable or leaving milestone notices.\n- [ ] **Complete native validation:** expand from START2A title/race acceptance to new/open/continue game, orders, turn generation, combat playback, reports, tutorial and error recovery.\n- [ ] **CRT data:** close the __fptaskdata zero-slot transformation and common PAD/EPAD placement, plus the explicit unresolved audit items. Code-member identity alone does not close data/link layout.\n- [ ] **Shared state/layout:** preserve globals, far-pointer representations, lifetime/ownership, zero-fill, tables and resource semantics across all callers.\n- [ ] **Dispatch closure:** audit every callback, entry point, pointer table and unresolved indirect target using [indirect-targets.json](analysis/decompiler/indirect-targets.json), [code-pointers.json](analysis/decompiler/code-pointers.json) and [runtime-dispatch.json](analysis/decompiler/runtime-dispatch.json).\n- [ ] **Uncovered ranges:** reconcile all non-procedure bytes with runtime code, embedded data, padding or genuinely missing functions. Do not invent one function per gap.\n- [ ] **Resources:** all 122 payloads are already embedded, but full use/semantics across dialogs, strings, menus and commands remains open.\n- [ ] **No-procedure modules:** retain debug/module records with no identified function bodies; an empty procedure list is not proof of no linked data or behavior.\n- [ ] **Final evidence:** resolve behavior-affecting uncertainties and run whole-program comparisons; byte-identical linking remains the separate optional milestone in TODO.md.')
    add('\n### Segment gap audit (not additional function counts)\n')
    add('The conservative source ledger and automatic instruction/data classifier have different purposes. The table summarizes conservative non-procedure code-segment ranges; use the linked ledgers for every exact interval. Runtime bodies above live inside some of these ranges. Do not add their bytes a second time.')
    add('\n| NE segment | Non-procedure ranges | Bytes |\n| --- | ---: | ---: |')
    gaps=collections.defaultdict(list)
    for r in ranges:
        if r['classification']=='unclassified_code_segment_range':gaps[r['ne_segment']].append(r)
    for seg,rr in sorted(gaps.items()):add(f'| `{seg:04x}` | {len(rr)} | {sum(r["bytes"] for r in rr):,} |')
    add('\nExact intervals: [source coverage ledger](analysis/coverage.json). Automated refinement: [decompiler coverage](analysis/decompiler/coverage.json). These classifications are evidence to review, not completed source.')
    add('\n### Runtime records without code spans\n')
    rd=read('analysis/reconstruction/runtime-data.json')
    for m in rd['modules']:
        if m['original_code_bytes']==0:add(f'- `{m["name"]}` — data audit status `{m["status"]}`; see [runtime-data.json](analysis/reconstruction/runtime-data.json).')
    add('\n### Module records without named game procedures\n')
    for ident,m in read('analysis/modules.json').items():
        if m['name'].endswith('.obj') and m['name'] not in modules:
            add(f'- `{m["name"]}` (module {ident}) — no named game procedure in this inventory; reconcile retained spans/data before overall closure.')
    add('\n## Keeping this inventory trustworthy\n')
    add('Update `tracking/reviews.json` and regenerate the coverage ledger using the existing project workflow before rerendering. Whole-function status is keyed by address, not just spelling. Update the generator’s partial-scope annotations when bounded implementations expand. The renderer asserts that all 846 game procedures join the 1,043-entry NE export exactly, all modules are categorized and every game signature is present. It preserves the old raw exports and does not change any reconstruction credit.')
    dest=ROOT/'TODO5.md';dest.write_text('\n'.join(out)+'\n')
    print(f'{dest.name}: {len(games)} game entries ({done} reviewed, {part} partial), {len(extras)} runtime entries ({mapped} mapped), 2 DOS entries; {len(out)} blocks.')

if __name__=='__main__':main()
