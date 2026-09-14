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

static int innate_fp_bits(InnateFP value)
{
    int bit;
    for (bit = 127; bit >= 0; --bit)
        if (innate_fp_bit(value, bit)) return bit + 1;
    return 0;
}

static InnateFP innate_fp_left(InnateFP value, int count)
{
    int i;
    while (count-- > 0) {
        for (i = 3; i > 0; --i)
            value.w[i] = (value.w[i] << 1) | (value.w[i - 1] >> 31);
        value.w[0] <<= 1;
    }
    return value;
}

static InnateFP innate_fp_right(InnateFP value, int count)
{
    int i;
    while (count-- > 0) {
        for (i = 0; i < 3; ++i)
            value.w[i] = (value.w[i] >> 1) | (value.w[i + 1] << 31);
        value.w[3] >>= 1;
    }
    return value;
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
    result.exponent = value.exponent;
    while (factor) {
        if (factor & 1U) result = innate_fp_sum_words(result, value);
        value = innate_fp_left(value, 1);
        factor >>= 1;
    }
    return innate_fp_round(result, 64, 0);
}

static InnateFP innate_fp_divide(InnateFP value, unsigned int divisor)
{
    InnateFP numerator;
    InnateFP result = innate_fp_integer(0);
    unsigned int remainder = 0;
    int i, shift = 128 - innate_fp_bits(value);
    if (shift == 128) return value;
    numerator = innate_fp_left(value, shift);
    result.exponent = value.exponent - shift;
    for (i = 127; i >= 0; --i) {
        remainder = remainder * 2U + (unsigned int)innate_fp_bit(numerator, i);
        if (remainder >= divisor) {
            remainder -= divisor;
            result.w[i / 32] |= (StarsU32)1 << (i % 32);
        }
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
