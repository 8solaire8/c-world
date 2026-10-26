#include <stdio.h>

#include "types.h"
#include "defs.h"
#include "lane.h"

thread_local LaneContext g_lane_context;

void
set_lane_context(LaneContext lc)
{
    g_lane_context = lc;
}

U64
lane_idx()
{
    return g_lane_context.id;
}

U64
lane_count()
{
    return g_lane_context.count;
}

void 
lane_sync()
{
    barrier_wait(g_lane_context.barrier);
}

LaneRange 
lane_range(U64 values_count)
{
    U64 thread_idx = lane_idx();
    U64 thread_count = lane_count();
    U64 values_per_thread = values_count / thread_count;
    U64 leftover_values_count = values_count % thread_count;
    U64 thread_has_leftover = (thread_idx < leftover_values_count);
    U64 leftovers_before_this_thread_idx = (thread_has_leftover
                                            ? thread_idx
                                            : leftover_values_count);
    U64 thread_first_value_idx = (values_per_thread * thread_idx +
                                  leftovers_before_this_thread_idx);
    U64 thread_opl_value_idx = (thread_first_value_idx + values_per_thread +
                                !!thread_has_leftover);
    
    return (LaneRange) {
        .min = thread_first_value_idx, .max = thread_opl_value_idx
    };
}

U64 
lane_width(LaneRange lr)
{
    return lr.max - lr.min;
}

void
lane_sync_u64(void* lane_local_storage, U64 broadcast_src_lane_idx)
{
    U64 broadcast_size = 8;
    
    if (lane_idx() == broadcast_src_lane_idx)
    {
        CopyMemory(g_lane_context.shared_buffer, lane_local_storage, broadcast_size);
    }
    lane_sync();
    
    if (lane_idx() != broadcast_src_lane_idx)
    {
        CopyMemory(lane_local_storage, g_lane_context.shared_buffer, broadcast_size);
    }
    lane_sync();
}
