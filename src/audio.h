/* date = November 2nd 2026 10:30 pm */

#ifndef AUDIO_H
#define AUDIO_H

#include "types.h"

typedef struct 
{
    U64 size;
    U32 id;
    F32* data;
} Sound;

typedef struct 
{
    U64  pos;
    U32 sound_id;
    bool finished;
} PlayingSound;

typedef struct 
{
    U32 sound_id;
    U32 volume;
    bool should_play;
} AudioCmd;


void load_sounds();
bool audio_init();
void start_audio_thread();

void play_sound(U32 id);


#endif //AUDIO_H
