#ifndef WIN32_AUDIO_H
#define WIN32_AUDIO_H

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
void play_sound(U32 id);

bool audio_create();
void audio_release();

void audio_thread_start();


#endif //WIN32_AUDIO_H
