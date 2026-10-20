/* date = October 20th 2026 8:37 pm */

#ifndef _LANE_H
#define _LANE_H
#include "types.h"
#define CPU_NUMBER 32


typedef struct
{
    u64 id;
    u64 count;
    
    u64* shared_buffer;
    Barrier* barrier;
} LaneContext;


typedef struct
{
    u64 min;
    u64 max;
} LaneRange;


u64 lane_count();
u64 lane_idx();

LaneRange lane_range(u64 values_count);

void lane_sync();
void lane_sync_u64(u64* lane_local_storage, u32 broad_cast_src_lane_idx);


#endif //_LANE_H
