#include <stdio.h>
#include <string.h>
#include "common.h"

typedef struct Case {
    StarsU16 group;
    StarsU16 item;
    StarsU16 base;
    StarsU16 stride;
} Case;

static const StarsU16 beam_fields[24][3] = {
    {1U,10U,9U}, {1U,16U,9U}, {2U,13U,12U}, {1U,26U,9U},
    {0U,90U,10U}, {2U,26U,7U}, {3U,82U,14U}, {3U,26U,5U},
    {2U,31U,12U}, {1U,66U,9U}, {0U,231U,10U}, {2U,66U,7U},
    {3U,211U,14U}, {3U,66U,5U}, {2U,80U,13U}, {1U,169U,9U},
    {0U,592U,11U}, {2U,169U,8U}, {3U,140U,6U}, {3U,541U,14U},
    {3U,169U,6U}, {2U,204U,13U}, {1U,433U,9U}, {2U,433U,8U}
};

static const StarsU16 torpedo_fields[12][3] = {
    {4U,5U,0U}, {4U,12U,1U}, {4U,26U,1U}, {5U,48U,2U},
    {5U,90U,2U}, {5U,169U,3U}, {5U,316U,4U}, {6U,60U,0U},
    {5U,85U,0U}, {5U,150U,1U}, {6U,280U,2U}, {6U,525U,3U}
};

static StarsU16 read_word(const StarsU8 *record, StarsU16 offset)
{
    return (StarsU16)(record[offset] | ((StarsU16)record[offset + 1U] << 8));
}

static StarsI16 read_segment(const char *path, StarsU8 *segment,
    StarsU16 *size)
{
    FILE *file;
    size_t count;
    int read_error;
    int close_status;

    file = fopen(path, "rb");
    if (!file)
        return 0;
    count = fread(segment, 1U, 25256U, file);
    read_error = ferror(file);
    if (!read_error && fgetc(file) != EOF)
        read_error = 1;
    close_status = fclose(file);
    if (read_error || close_status != 0 || count != 25256U)
        return 0;
    *size = (StarsU16)count;
    return 1;
}

static StarsU8 segment[25256];

int lookup_part_memory_probe(const char *path)
{
    static const Case cases[16] = {
        {0x0001U,3U,0x0000U,0x004eU}, {0x0002U,4U,0x0768U,0x0038U},
        {0x0004U,5U,0x0ae8U,0x0036U}, {0x0008U,3U,0x04e0U,0x0036U},
        {0x0010U,7U,0x2450U,0x003cU}, {0x0020U,7U,0x2180U,0x003cU},
        {0x0040U,8U,0x1e1aU,0x003aU}, {0x0080U,5U,0x1508U,0x0036U},
        {0x0100U,2U,0x12ecU,0x0036U}, {0x0200U,15U,0x4d8aU,0x0038U},
        {0x0400U,4U,0x4872U,0x008fU}, {0x0800U,9U,0x0d04U,0x0036U},
        {0x1000U,4U,0x109aU,0x0036U}, {0x2000U,7U,0x19e2U,0x0036U},
        {0x4000U,20U,0x29f0U,0x008fU}, {0x8000U,14U,0x16b8U,0x0036U}
    };
    StarsU16 segment_size;
    StarsLookupPartInput input;
    const StarsU8 *record;
    StarsU16 family;
    StarsU16 item;
    StarsU16 field;
    StarsU16 offset;
    StarsI16 failures;
    StarsU16 checks;

    if (!path || !read_segment(path, segment, &segment_size) ||
            segment_size != 25256U)
        return 2;
    failures = 0;
    checks = 0U;
    for (family = 0U; family < 16U; ++family) {
        memset(&input, 0, sizeof input);
        input.group = cases[family].group;
        input.item = cases[family].item;
        input.player = -1;
        /* FLookupPart writes the table far pointer before race/tech gates. */
        (void)stars_lookup_part(&input);
        record = stars_lookup_part_record(&input, segment, segment_size);
        ++checks;
        if (record != segment + input.table_offset ||
                input.table_base_offset != cases[family].base ||
                input.table_stride != cases[family].stride)
            ++failures;
    }
    for (item = 0U; item < 24U; ++item) {
        memset(&input, 0, sizeof input);
        input.group = 0x0010U;
        input.item = item;
        input.player = -1;
        (void)stars_lookup_part(&input);
        record = stars_lookup_part_record(&input, segment, segment_size);
        for (field = 0U; field < 3U; ++field) {
            offset = (StarsU16)(52U + field * 2U);
            ++checks;
            if (!record || read_word(record, offset) != beam_fields[item][field])
                ++failures;
        }
    }
    for (item = 0U; item < 12U; ++item) {
        memset(&input, 0, sizeof input);
        input.group = 0x0020U;
        input.item = item;
        input.player = -1;
        (void)stars_lookup_part(&input);
        record = stars_lookup_part_record(&input, segment, segment_size);
        for (field = 0U; field < 3U; ++field) {
            offset = (StarsU16)(52U + field * 2U);
            ++checks;
            if (!record || read_word(record, offset) !=
                    torpedo_fields[item][field])
                ++failures;
        }
    }
    memset(&input, 0, sizeof input);
    input.group = 0x0010U;
    input.item = 23U;
    input.player = -1;
    (void)stars_lookup_part(&input);
    ++checks;
    if (stars_lookup_part_record(&input, segment,
            (StarsU16)(input.table_offset + input.table_stride - 1U)) != 0)
        ++failures;
    input.table_segment_is_current_cs = 0U;
    ++checks;
    if (stars_lookup_part_record(&input, segment, segment_size) != 0)
        ++failures;
    input.table_segment_is_current_cs = 1U;
    ++input.table_base_offset;
    ++checks;
    if (stars_lookup_part_record(&input, segment, segment_size) != 0)
        ++failures;
    --input.table_base_offset;
    ++input.table_offset;
    ++checks;
    if (stars_lookup_part_record(&input, segment, segment_size) != 0)
        ++failures;
    --input.table_offset;
    ++input.table_stride;
    ++checks;
    if (stars_lookup_part_record(&input, segment, segment_size) != 0)
        ++failures;
#ifndef TARGET_NATIVE
    printf("LOOKUP_PART_MEMORY_CHECKS=%u FAILURES=%d RESULT=%s\n",
        (unsigned int)checks, failures, failures ? "FAIL" : "PASS");
#else
    (void)checks;
#endif
    return failures;
}

#ifndef TARGET_NATIVE
int main(int argc, char **argv)
{
    if (argc != 2)
        return 2;
    return lookup_part_memory_probe(argv[1]);
}
#endif
