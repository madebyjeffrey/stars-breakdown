"""Original X2 framing regression; serial/config are explicit codec inputs.

This does not supply original output header fields to the turn coordinator.
"""
import subprocess
import tempfile
import unittest
from pathlib import Path

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream

ROOT = Path(__file__).resolve().parents[1]


class OriginalLogFileTests(unittest.TestCase):
    def test_original_x2_load_and_write(self):
        inspector = Inspector()
        for run in ("vmctl-sye3k0", "vmctl-sye3k1"):
            with self.subTest(run=run), tempfile.TemporaryDirectory() as directory:
                records = decode_stream(ROOT / "runs" / run / "BACKUP/CLEAN.X2", inspector)
                self.assertEqual([r["kind"] for r in records],
                                 [8, 9, 27, 27, 27, 27, 34, 23, 29, 4, 1, 5, 0])
                bodies = [bytes(r["body"]) for r in records]
                memory = b"".join(((r["kind"] << 10) | len(body)).to_bytes(2, "little") + body
                                  for r, body in zip(records[2:-1], bodies[2:-1]))
                self.assertEqual(int.from_bytes(bodies[1][:2], "little"), len(memory))
                declarations = []
                for index, body in enumerate(bodies + [memory]):
                    declarations.append("static const StarsU8 b%d[] = {%s};" %
                                        (index, ",".join(str(v) for v in body) or "0"))
                declarations.append("static StarsLoadGameRecord records[] = {%s};" %
                                    ",".join("{%d,%d,b%d}" % (r["kind"], len(body), i)
                                             for i, (r, body) in enumerate(zip(records, bodies))))
                source = '\n'.join(declarations)
                source = '#include "core.h"\n#include <string.h>\n' + source + r'''
static StarsU16 errors, writes, closed, dirty, serials;
static StarsU16 word(const StarsU8 *p) { return (StarsU16)(p[0] | ((StarsU16)p[1]<<8)); }
static StarsU32 lng(const StarsU8 *p) { return word(p) | ((StarsU32)word(p+2)<<16); }
static StarsI16 serial(void *c, const StarsLoadGameRecord *r) {
    (void)c; ++serials;
    if(r->type!=9U || r->length!=17U || memcmp(r->data,b1,17U)) ++errors;
    return 1;
}
static void err(void *c, StarsI16 e) { (void)c; (void)e; ++errors; }
static StarsI16 create(void *c, StarsI16 p, const StarsU8 *b) {
    (void)c; (void)p; (void)b; return 1;
}
static void write_record(void *c, StarsU16 t, StarsU16 n, const void *b) {
    const StarsLoadGameRecord *r; (void)c;
    if(writes>=12U) { ++errors; return; }
    r=&records[++writes];
    if(t!=r->type || n!=r->length || (n && memcmp(b,r->data,n))) ++errors;
}
static void close_stream(void *c) { (void)c; ++closed; }
static void dirty_game(void *c) { (void)c; ++dirty; }
int main(void) {
    StarsLoadLogFileInput load;
    StarsLoadLogFileState state;
    StarsWriteLogFileInput save;
    StarsU8 memory[2048];
    StarsU16 flags=0;
    memset(&load,0,sizeof load); memset(&save,0,sizeof save);
    load.records=records; load.record_count=13U;
    load.game_id=lng(b0+4); load.game_turn=word(b0+10);
    load.game_turn_flags=(StarsU16)(((word(b0+14)>>13)&7U)<<9);
    load.log=memory; load.log_capacity=sizeof memory;
    load.serial=serial; load.file_error=err;
    if(!stars_load_log_file(&load,&state) || errors || serials!=1U ||
       state.log_length!=sizeof b13 || memcmp(memory,b13,sizeof b13)) return 1;
    save.player=1; save.current_player=-1;
    save.serial=lng(b1+2); save.environment=b1+6;
    save.log=memory; save.log_size=state.log_length;
    save.create_file=create; save.write_record=write_record;
    save.close_stream=close_stream; save.dirty_game=dirty_game;
    save.game_flags=&flags;
    if(!stars_write_log_file(&save) || errors || writes!=12U ||
       closed!=1U || dirty!=1U || flags!=0x100U) return 2;
    /* The original BOF magic must never be interpreted as cbLog. */
    records[1].type=17U;
    if(stars_load_log_file(&load,&state) || errors!=1U) return 3;
    return 0;
}
'''
                path = Path(directory)
                (path / "probe.c").write_text(source)
                command = ["cc", "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                           "-fsanitize=address", "-I", str(ROOT / "reconstructed/include"),
                           str(path / "probe.c"), str(ROOT / "reconstructed/src/load_log_file.c"),
                           str(ROOT / "reconstructed/src/write_log_file.c"), "-o", str(path / "probe")]
                subprocess.run(command, check=True, capture_output=True, text=True)
                result = subprocess.run([str(path / "probe")], capture_output=True, text=True)
                self.assertEqual(result.returncode, 0, result.stderr)
