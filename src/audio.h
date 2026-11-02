/* date = November 2nd 2026 10:30 pm */

#ifndef AUDIO_H
#define AUDIO_H

#include "types.h"

typedef struct 
{
    F32* buffer;
    size_t size;
} Sound;

typedef struct 
{
    U32 sound_id;
    U64  pos;
    bool finished;
} PlayingSound;

typedef struct 
{
    U32 sound_id;
    U32 volume;
    bool should_play;
} AudioCmd;


bool audio_init();

void play_sound(U32 id);


#endif //AUDIO_H
