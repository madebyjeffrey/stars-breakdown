/* Positive bounded x87 arithmetic used by LInnateRaceHabitability.
 * Value = four little-endian 32-bit limbs * 2^exponent. Operations round
 * to 64 significand bits, stores to 53, nearest with ties to even.
 * No native floating-point arithmetic or non-C89 integer types are used.
 * This is not a general x87 emulator: no negative/nonfinite/subnormal values.
 * Operand exponent separation must leave the aligned sum within 128 bits;
 * the sampler's nonzero accumulators satisfy this bound. */
typedef struct InnateFP { StarsU32 w[4]; int exponent; } InnateFP;

static InnateFP innate_fp_integer(StarsU32 value)
{
    InnateFP result;
    result.w[0] = value;
    result.w[1] = result.w[2] = result.w[3] = 0;
    result.exponent = 0;
    return result;
}

static int innate_fp_bit(InnateFP value, int bit)
{
    return (int)((value.w[bit / 32] >> (bit % 32)) & 1U);
}

/* Word operations avoid hundreds of single-bit shifts per x87 operation on
 * a 16-bit compiler. Every intermediate still uses the same integer value. */
static int innate_fp_bits(InnateFP value)
{
    int i, bits;
    StarsU32 word;
    for (i = 3; i >= 0; --i) {
        word = value.w[i];
        if (!word) continue;
        bits = i * 32 + 1;
        if (word >> 16) { bits += 16; word >>= 16; }
        if (word >> 8) { bits += 8; word >>= 8; }
        if (word >> 4) { bits += 4; word >>= 4; }
        if (word >> 2) { bits += 2; word >>= 2; }
        if (word >> 1) ++bits;
        return bits;
    }
    return 0;
}

static InnateFP innate_fp_left(InnateFP value, int count)
{
    InnateFP result = innate_fp_integer(0);
    int i, words, bits;
    if (count <= 0) return value;
    result.exponent = value.exponent;
    if (count >= 128) return result;
    words = count / 32; bits = count % 32;
    for (i = words; i < 4; ++i) {
        result.w[i] = value.w[i - words] << bits;
        if (bits && i > words)
            result.w[i] |= value.w[i - words - 1] >> (32 - bits);
    }
    return result;
}

static InnateFP innate_fp_right(InnateFP value, int count)
{
    InnateFP result = innate_fp_integer(0);
    int i, words, bits;
    if (count <= 0) return value;
    result.exponent = value.exponent;
    if (count >= 128) return result;
    words = count / 32; bits = count % 32;
    for (i = 0; i + words < 4; ++i) {
        result.w[i] = value.w[i + words] >> bits;
        if (bits && i + words + 1 < 4)
            result.w[i] |= value.w[i + words + 1] << (32 - bits);
    }
    return result;
}

static InnateFP innate_fp_sum_words(InnateFP a, InnateFP b)
{
    StarsU32 first, second, carry = 0, next;
    int i;
    for (i = 0; i < 4; ++i) {
        first = a.w[i] + b.w[i];
        next = first < a.w[i];
        second = first + carry;
        carry = next | (second < first);
        a.w[i] = second;
    }
    return a;
}

static InnateFP innate_fp_round(InnateFP value, int precision, int sticky)
{
    int shift = innate_fp_bits(value) - precision;
    int i, guard;
    if (shift <= 0) return value;
    guard = innate_fp_bit(value, shift - 1);
    for (i = 0; i < shift - 1; ++i) sticky |= innate_fp_bit(value, i);
    value = innate_fp_right(value, shift);
    value.exponent += shift;
    if (guard && (sticky || (value.w[0] & 1U))) {
        value = innate_fp_sum_words(value, innate_fp_integer(1));
        if (innate_fp_bits(value) > precision) {
            value = innate_fp_right(value, 1);
            ++value.exponent;
        }
    }
    return value;
}

static InnateFP innate_fp_add(InnateFP a, InnateFP b)
{
    if (!innate_fp_bits(a)) return b;
    if (!innate_fp_bits(b)) return a;
    if (a.exponent > b.exponent) {
        a = innate_fp_left(a, a.exponent - b.exponent);
        a.exponent = b.exponent;
    } else {
        b = innate_fp_left(b, b.exponent - a.exponent);
        b.exponent = a.exponent;
    }
    return innate_fp_round(innate_fp_sum_words(a, b), 64, 0);
}

static InnateFP innate_fp_multiply(InnateFP value, unsigned int factor)
{
    InnateFP result = innate_fp_integer(0);
    StarsU32 part, carry = 0;
    int i, shift;
    result.exponent = value.exponent;
    for (i = 0; i < 8; ++i) {
        shift = (i % 2) * 16;
        part = ((value.w[i / 2] >> shift) & 65535UL) * factor + carry;
        result.w[i / 2] |= (part & 65535UL) << shift;
        carry = part >> 16;
    }
    return innate_fp_round(result, 64, 0);
}

static InnateFP innate_fp_divide(InnateFP value, unsigned int divisor)
{
    InnateFP numerator;
    InnateFP result = innate_fp_integer(0);
    StarsU32 part, remainder = 0;
    int i, bits, shift = 128 - innate_fp_bits(value);
    if (shift == 128) return value;
    numerator = innate_fp_left(value, shift);
    result.exponent = value.exponent - shift;
    /* Base-65536 long division. Divisors and factors fit a Win16 unsigned
     * int, so the remainder/next-digit intermediate fits 32 bits. */
    for (i = 7; i >= 0; --i) {
        bits = (i % 2) * 16;
        part = (remainder << 16) | ((numerator.w[i / 2] >> bits) & 65535UL);
        result.w[i / 2] |= (part / divisor) << bits;
        remainder = part % divisor;
    }
    return innate_fp_round(result, 64, remainder != 0);
}

static InnateFP innate_fp_store(InnateFP value)
{
    return innate_fp_round(value, 53, 0);
}

static InnateFP innate_fp_scale(InnateFP value, StarsI16 center, StarsI16 span)
{
    value = innate_fp_multiply(value, center < 0 ? 11U : (unsigned int)span);
    if (center >= 0) value = innate_fp_divide(value, 100U);
    return innate_fp_store(value);
}

static StarsI32 innate_fp_result(InnateFP total)
{
    InnateFP half = innate_fp_integer(1);
    half.exponent = -1;
    total = innate_fp_add(innate_fp_divide(total, 10U), half);
    if (total.exponent < 0) total = innate_fp_right(total, -total.exponent);
    else total = innate_fp_left(total, total.exponent);
    return (StarsI32)total.w[0];
}
