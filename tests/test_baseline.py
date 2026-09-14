"""Validate inventory identity and exhaustive, disjoint coverage partitioning."""
import json
from pathlib import Path
import unittest
from tools.baseline import inventory, coverage

ROOT=Path(__file__).resolve().parents[1]

class BaselineChecks(unittest.TestCase):
    def test_retained_inventory_and_archive_identity(self):
        actual=inventory()
        self.assertEqual(actual,json.loads((ROOT/'analysis/input_inventory.json').read_text()))
        exe=next(m for m in actual['archive_members'] if m['member']=='stars.exe')
        self.assertEqual(exe['matching_files'],['target/stars.exe'])

    def test_coverage_partitions_every_segment(self):
        ledger=coverage()
        self.assertEqual(ledger,json.loads((ROOT/'analysis/coverage.json').read_text()))
        segments=json.loads((ROOT/'analysis/segments.json').read_text())
        for seg in segments:
            rows=[r for r in ledger['ranges'] if r['ne_segment']==seg['segment']]
            cursor=0
            for row in rows:
                self.assertEqual(row['start'],cursor)
                self.assertGreater(row['end_exclusive'],cursor)
                cursor=row['end_exclusive']
            self.assertEqual(cursor,max(seg['size'],seg['allocation']))
        self.assertEqual(ledger['summary']['manually_reviewed_bytes'],1905)
        self.assertEqual(ledger['summary']['compiled_reconstruction_bytes'],1905)
        compiled=[r for r in ledger['ranges'] if r.get('compilation_status')=='host_c89_compiled']
        self.assertEqual(len(compiled),15)
        for row in compiled:
            for evidence in row['evidence']:
                self.assertTrue((ROOT/evidence).is_file(),evidence)

    def test_unclassified_gaps_are_not_reported_as_reviewed(self):
        gaps=[r for r in coverage()['ranges'] if r['classification']=='unclassified_code_segment_range']
        self.assertTrue(gaps)
        for gap in gaps:
            self.assertEqual(gap['review_status'],'not_started')
            self.assertIsNone(gap['reconstruction_path'])

if __name__=='__main__': unittest.main()
