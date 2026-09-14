#!/usr/bin/env python3
"""Copy the closed Win9x transfer disk to a new host directory with mtools."""
import argparse
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('destination', type=Path, help='New directory; existing outputs are preserved')
    args = parser.parse_args()
    disk = ROOT / 'runs/step4/transfer.img'
    mcopy = shutil.which('mcopy')
    destination = args.destination.resolve()
    if not disk.is_file() or not mcopy:
        parser.error('Transfer disk or mtools is missing (brew install mtools)')
    # DOSBox retains the image even after Windows shuts down. Refuse to open
    # it while another process owns it; an unexpected lsof error also blocks.
    check = subprocess.run(['/usr/sbin/lsof', '-t', str(disk)], capture_output=True, text=True)
    if check.returncode != 1 or check.stdout.strip() or check.stderr.strip():
        parser.error('Close the DOSBox instance using transfer.img before exporting (or check lsof permissions)')
    with disk.open('rb') as image:
        mbr = image.read(512)
    if mbr[510:512] != b'\x55\xaa':
        parser.error('Transfer disk has no valid MBR signature')
    partitions = [mbr[n:n+16] for n in range(446, 510, 16) if mbr[n+4]]
    if len(partitions) != 1 or partitions[0][4] not in [1, 4, 6, 11, 12, 14]:
        parser.error('Expected one FAT partition on the transfer disk')
    offset = int.from_bytes(partitions[0][8:12], 'little') * 512
    size = int.from_bytes(partitions[0][12:16], 'little') * 512
    if offset < 512 or not size or offset + size > disk.stat().st_size:
        parser.error('Invalid transfer partition bounds')
    destination.mkdir(parents=True, exist_ok=False)
    subprocess.run([mcopy, '-i', f'{disk}@@{offset}', '-s', '::*', str(destination)], check=True)
    if not (destination / 'README.TXT').is_file() or not (destination / 'START2A/START.EXE').is_file():
        raise SystemExit('Export incomplete; preserve the destination for inspection')
    print(f'Exported transfer disk to {destination}')
    print('Returned test folders are under RETURN; keep the disk until their contents are verified.')


if __name__ == '__main__':
    main()
