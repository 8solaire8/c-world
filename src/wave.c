#include <string.h>
#include "types.h"


local_function void 
write_u16(FILE* f, U16 v) 
{
    fputc(v & 0xFF, f);
    fputc((v >> 8) & 0xFF, f);
}

local_function void 
write_u32(FILE* f, U32 v) 
{
    for (U32 i = 0; i < 4; i++)
        fputc((v >> (8 * i)) & 0xFF, f);
}

U32
write_wav_f32(const char* filename, const F32* samples, U32 count, U32 sample_rate)
{
    if (!filename || (!samples && count) ||
        sample_rate == 0 ||
        sample_rate > UINT32_MAX / 4 ||
        count > (UINT32_MAX - 48) / 4)
    {
        return -1;
    }
    
    FILE* f = fopen(filename, "wb");
    if (!f) return -1;

    U32 data_size = count * 4;

    fwrite("RIFF", 1, 4, f);
    write_u32(f, 48 + data_size);
    fwrite("WAVE", 1, 4, f);

    // Format chunk
    fwrite("fmt ", 1, 4, f);
    write_u32(f, 16);
    write_u16(f, 3);               // IEEE float
    write_u16(f, 1);               // Mono
    write_u32(f, sample_rate);
    write_u32(f, sample_rate * 4);
    write_u16(f, 4);               // Block align
    write_u16(f, 32);              // Bits per sample

    // Fact chunk
    fwrite("fact", 1, 4, f);
    write_u32(f, 4);
    write_u32(f, count);

    // Audio data
    fwrite("data", 1, 4, f);
    write_u32(f, data_size);

    // Assumes IEEE 754 32-bit float
    for (U32 i = 0; i < count; i++) {
        U32 bits;
        memcpy(&bits, &samples[i], 4);
        write_u32(f, bits);
    }

    U32 ok = !ferror(f);
    if (fclose(f) != 0) ok = 0;
    return ok ? 0 : -1;
}


U32
read_wave_file(const char* filename, F32* buffer)
{
    FILE* f = fopen(filename, "wb");

    return 1;
}