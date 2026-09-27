#!/usr/bin/env python3
"""Build the reviewed C89 semantic core as a host shared library."""
import argparse
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
SOURCES = ['utilgen.c', 'race.c', 'near_state.c', 'codec.c', 'text.c', 'create.c',
           'planet.c', 'strings.c', 'records.c', 'stream.c', 'file.c', 'alerts.c',
           'runtime.c', 'password.c', 'output.c', 'format.c', 'ui.c', 'raceui.c',
           'racedraw.c', 'racewizard.c']
# Keep the frozen race-export source set stable; foundation modules join the
# host library without silently changing the already accepted native package.
FOUNDATION_SOURCES = ['memory.c', 'common.c', 'change_population.c', 'startup.c', 'resources.c', 'title.c', 'config.c', 'lookup_part.c', 'true_part_cost.c', 'reprice_shdef.c', 'read_player_messages.c']
MDI_SOURCES = ['mdi.c', 'child_windows.c', 'refit.c', 'ensure_ais.c', 'menu.c', 'invert.c', 'verify_turns.c', 'bringup_host.c', 'draw_host.c', 'host_timer.c', 'host_mode.c', 'open_game.c', 'frame.c', 'winmain.c', 'title_wnd.c', 'create_stuff.c', 'read_ini.c', 'write_ini.c', 'mine_simple.c', 'mine_selection.c', 'mine_bars.c', 'mine_title.c', 'messages.c', 'ai.c', 'ai_plord.c', 'tb.c', 'produce.c', 'scan.c', 'turn2.c', 'ship.c', 'popup.c', 'battle.c']


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cc', default='cc')
    args = parser.parse_args()
    output = ROOT / 'build' / ('libstars_core.dylib' if sys.platform == 'darwin' else 'libstars_core.so')
    output.parent.mkdir(exist_ok=True)
    subprocess.run([args.cc, '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                    '-O2', '-shared', '-fPIC', '-I', str(ROOT / 'reconstructed/include'),
                    *[str(ROOT / 'reconstructed/src' / name) for name in SOURCES + FOUNDATION_SOURCES + MDI_SOURCES], '-o', str(output)], check=True)
    print(output)


if __name__ == '__main__':
    main()
