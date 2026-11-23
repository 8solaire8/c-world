#ifndef AUDIO_WAV_H
#define AUDIO_WAV_H

internal U32 write_wav_f32(const char* filename, const F32* samples, U32 count, U32 sample_rate);

internal U64 wav_size(String8 file_name);

internal void wav_load(F32* output, String8 file_name);

#endif //AUDIO_WAV_H