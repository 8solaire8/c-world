#include "types.h"
#include "str.h"

U32 write_wav_f32(const char* filename, const F32* samples, U32 count, U32 sample_rate);

U64 wav_size(String8 file_name);

void wav_load(F32* output, String8 file_name);
