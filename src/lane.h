#ifndef LANE_H
#define LANE_H

#include "platform.h"
#include "types.h"
#include "Arena.h"

extern thread_local Context context;

struct Context
{
    U64 id;
    U64 count;
    
    U64* shared_buffer;
    Barrier* barrier;
    
    Arena arena;
};


struct LaneRange
{
    U64 min;
    U64 max;
};


void context_create(U64 id, U64 count);

U64 lane_count();
U64 lane_idx();

LaneRange lane_range(U64 values_count);

U64 lane_width(LaneRange lr);
void lane_sync();
void lane_sync_u64(void* lane_local_storage, U64 broad_cast_src_lane_idx);


#endif //_LANE_H
