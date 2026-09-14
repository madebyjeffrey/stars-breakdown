"""Bounded positive x87 arithmetic checked against exact rational rounding."""
import ctypes as C
from fractions import Fraction as F
from pathlib import Path
import random
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]

class FP(C.Structure):
    _fields_ = [('w', C.c_uint32 * 4), ('exponent', C.c_int)]


def value(x):
    return F(sum(int(v) << (32*i) for i,v in enumerate(x.w))) * F(2)**x.exponent


def rounded(x, precision):
    if not x: return x
    exponent = x.numerator.bit_length() - x.denominator.bit_length()
    if x < F(2)**exponent: exponent -= 1
    unit = F(2)**(exponent - precision + 1)
    n, remainder = divmod(x / unit, 1)
    return (int(n) + (remainder > F(1,2) or (remainder == F(1,2) and int(n) % 2))) * unit


class InnateFPTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        source = Path(cls.temp.name) / 'fp.c'
        source.write_text('''#include "core.h"
#include "innate_fp.h"
void probe(InnateFP *out, InnateFP *a, InnateFP *b, int op, unsigned int n) {
    if (op == 0) *out = innate_fp_add(*a, *b);
    else if (op == 1) *out = innate_fp_multiply(*a, n);
    else if (op == 2) *out = innate_fp_divide(*a, n);
    else if (op == 3) *out = innate_fp_store(*a);
    else if (op == 4) *out = innate_fp_scale(*a, 50, (StarsI16)n);
    else if (op == 6) *out = innate_fp_left(*a, (int)n);
    else if (op == 7) *out = innate_fp_right(*a, (int)n);
    else *out = innate_fp_integer((StarsU32)innate_fp_result(*a));
}
''')
        library = Path(cls.temp.name) / 'fp.so'
        subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2',
                        '-shared','-fPIC','-I'+str(ROOT/'reconstructed/include'),
                        '-I'+str(ROOT/'reconstructed/src'),str(source),'-o',str(library)], check=True)
        cls.lib = C.CDLL(str(library))
        cls.lib.probe.argtypes = [C.POINTER(FP)]*3 + [C.c_int,C.c_uint]

    @classmethod
    def tearDownClass(cls): cls.temp.cleanup()

    def check(self, mantissa, exponent, second, second_exponent, n):
        a = FP((C.c_uint32*4)(mantissa & 0xffffffff, mantissa >> 32, 0, 0), exponent)
        b = FP((C.c_uint32*4)(second & 0xffffffff, second >> 32, 0, 0), second_exponent)
        x, y = value(a), value(b)
        expected = [rounded(x+y,64), rounded(x*n,64), rounded(x/n,64), rounded(x,53),
                    rounded(rounded(rounded(x*n,64)/100,64),53)]
        for op, want in enumerate(expected):
            out = FP()
            self.lib.probe(C.byref(out), C.byref(a), C.byref(b), op, n)
            self.assertEqual(value(out), want, (mantissa,exponent,second,second_exponent,n,op))

    def test_random_exact_rational_operations(self):
        rng = random.Random(270018)
        for _ in range(1000):
            self.check(rng.getrandbits(rng.randrange(1,65)), rng.randrange(-60,-20),
                       rng.getrandbits(rng.randrange(1,65)), rng.randrange(-60,-20), rng.randrange(1,101))

    def test_carries_ties_small_quotients_and_zero(self):
        for mantissa in [0,1,2,3, (1<<53)-1, (1<<53)+1, (1<<53)+3, (1<<64)-1]:
            for n in [1,10,11,100,32767]: self.check(mantissa,-32,1,-32,n)

    def test_word_shift_boundaries(self):
        rng = random.Random(270023)
        for _ in range(50):
            number = rng.getrandbits(128)
            a = FP((C.c_uint32*4)(*[(number >> (32*i)) & 0xffffffff for i in range(4)]), -53)
            b, out = FP(), FP()
            for shift in [0,1,15,16,31,32,33,63,64,65,95,96,97,127,128,160]:
                for op, want in [(6,(number << shift) & ((1<<128)-1)), (7,number >> shift)]:
                    self.lib.probe(C.byref(out),C.byref(a),C.byref(b),op,shift)
                    self.assertEqual(value(out),F(want)*F(2)**-53)
                    self.assertEqual(out.exponent,-53)

    def test_final_half_boundaries(self):
        for integer in [0,1,10,1043305,23957999]:
            # Nearby representable binary64 totals, then x87 divide/add/trunc.
            for adjustment in [-1,0,1]:
                mantissa = (integer*10+5)*(1<<24) + adjustment
                a = FP((C.c_uint32*4)(mantissa & 0xffffffff,mantissa>>32,0,0), -24)
                b, out = FP(), FP()
                self.lib.probe(C.byref(out),C.byref(a),C.byref(b),5,0)
                expected = int(rounded(rounded(value(a)/10,64)+F(1,2),64))
                self.assertEqual(value(out),expected)

if __name__ == '__main__': unittest.main()
