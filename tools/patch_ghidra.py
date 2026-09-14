#!/usr/bin/env python3
"""Patch only the isolated, pinned Ghidra compiler specification for Win16."""
from pathlib import Path
import hashlib
p=Path(__file__).resolve().parents[1]/'.tools/ghidra_12.1.3_PUBLIC/Ghidra/Processors/x86/data/languages/x86-16.cspec'
backup=p.with_suffix('.cspec.original')
if not backup.exists():backup.write_bytes(p.read_bytes())
s=backup.read_text()
for name in ('__cdecl16far','__stdcall16far'):
 start=s.index('<prototype name="'+name+'"');end=s.index('</prototype>',start)
 t=s[start:end]
 if 'piece1="DX"' not in t:t=t.replace('</output>','  <pentry minsize="3" maxsize="4"><addr space="join" piece1="DX" piece2="AX"/></pentry>\n    </output>')
 s=s[:start]+t+s[end:]
# ES is scratch for LES-based far data access; do not propagate it across calls.
s=s.replace('      <register name="ES"/>\n','')
p.write_text(s)
print('Win16 compiler-spec patch:',hashlib.sha256(p.read_bytes()).hexdigest())
# Diagnostic only: report the storage address if upstream variable merging fails.
cpp=p.parents[4]/'Features/Decompiler/src/decompile/cpp/merge.cc'
s=cpp.read_text()
s=s.replace('throw LowlevelError("Forced merge caused intersection");', '{ std::ostringstream diag; diag << "Forced merge caused intersection: "; vn->printRaw(diag); throw LowlevelError(diag.str()); }')
cpp.write_text(s)
