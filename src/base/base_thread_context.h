#ifndef BASE_THREAD_CONTEXT_H
#define BASE_THREAD_CONTEXT_H

typedef struct Context Context;
struct Context
{
    U64 id;
    U64 count;
    
    U64 *shared_buffer;
    Barrier barrier;
    
    Arena *arena;
};

extern thread_static Context context;

typedef struct LaneRange LaneRange;
struct LaneRange
{
    U64 min;
    U64 max;
};


internal void context_create(U64 id, U64 count);

internal U64 lane_count();
internal U64 lane_idx();

internal LaneRange lane_range(U64 values_count);

internal U64 lane_width(LaneRange lr);
internal void lane_sync();
internal void lane_sync_u64(RawPtr lane_local_storage, U64 broad_cast_src_lane_idx);

internal ArenaTemp scratch_begin();
#define scratch_end(t) arena_temp_end(t)

#endif // BASE_THREAD_CONTEXT_H
