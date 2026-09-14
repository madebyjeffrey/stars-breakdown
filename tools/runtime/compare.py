#!/usr/bin/env python3
"""Snapshot selected game files; compare complete bytes without exposing contents."""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
EXTENSION = re.compile(r'\.(?:hst|xy|[mrxh](?:[1-9]|1[0-6]))$', re.I)


def game_file(root, name):
    p = Path(name)
    if p.name != name or not EXTENSION.fullmatch(p.suffix):
        raise ValueError('Select individual game/race files by basename')
    source = (root / p).resolve()
    if source.parent != root.resolve():
        raise ValueError('Fixture escapes selected directory')
    return source


def signature(name, data):
    return dict(name=name, size=len(data), sha256=hashlib.sha256(data).hexdigest())


def capture(root, names, dest):
    dest = dest.resolve()
    if not dest.is_relative_to((ROOT / 'runs').resolve()):
        raise ValueError('Raw fixtures must remain under runs/')
    if not names or len(set(names)) != len(names):
        raise ValueError('Select a nonempty, unique file list')
    selected = [(name, game_file(root, name).read_bytes()) for name in names]
    dest.mkdir(parents=True, exist_ok=False)
    for name, data in selected:
        (dest / name).write_bytes(data)
    result = dict(files=[signature(n, d) for n, d in selected], normalization='none')
    (dest / 'manifest.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


def verified_snapshot(root):
    manifest = json.loads((root / 'manifest.json').read_text())
    if manifest.get('normalization') != 'none' or not manifest['files']:
        raise ValueError('Expected a nonempty, unnormalized snapshot')
    result = {}
    for row in manifest['files']:
        name = row['name']
        if name in result:
            raise ValueError('Duplicate fixture entry')
        data = game_file(root, name).read_bytes()
        if signature(name, data) != row:
            raise ValueError('Fixture does not match its recorded identity')
        result[name] = data
    return result


def compare(left, right):
    a, b = verified_snapshot(left), verified_snapshot(right)
    rows = []
    for name in sorted(a.keys() | b.keys()):
        if name not in a or name not in b:
            rows.append(dict(name=name, equal=False, reason='missing in one run'))
            continue
        x, y = a[name], b[name]
        differences = [i for i, (u, v) in enumerate(zip(x, y)) if u != v]
        first = differences[0] if differences else min(len(x), len(y)) if x != y else None
        rows.append(dict(name=name, equal=x == y, left_size=len(x), right_size=len(y),
                         differing_bytes=len(differences) + abs(len(x) - len(y)), first_difference=first))
    return dict(equal=all(r['equal'] for r in rows), normalization='none', files=rows)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    sub = ap.add_subparsers(dest='action', required=True)
    cap = sub.add_parser('capture')
    cap.add_argument('root', type=Path)
    cap.add_argument('destination', type=Path)
    cap.add_argument('files', nargs='+')
    cmp = sub.add_parser('compare')
    cmp.add_argument('left', type=Path)
    cmp.add_argument('right', type=Path)
    cmp.add_argument('--output', type=Path)
    args = ap.parse_args()
    result = capture(args.root, args.files, args.destination) if args.action == 'capture' else compare(args.left, args.right)
    text = json.dumps(result, indent=2) + '\n'
    if getattr(args, 'output', None):
        args.output.write_text(text)
    print(text, end='')
    if args.action == 'compare' and not result['equal']:
        raise SystemExit(1)

if __name__ == '__main__':
    main()
