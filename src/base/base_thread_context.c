thread_local Context context;
global Barrier barrier;
global U64 shared_buffer;

ArenaTemp 
scratch_begin()
{
    ArenaTemp temp = {0};
    temp.arena = &context.arena;
    temp.pos = context.arena.ptr;
    return temp;
}

void
context_create(U64 id, U64 count)
{
    context.id = id;
    context.count = count;
    context.shared_buffer = &shared_buffer;
    context.barrier = &barrier;
    arena_create(&context.arena, GIGABYTES(4));
}

U64
lane_idx()
{
    return context.id;
}

U64
lane_count()
{
    return context.count;
}

void 
lane_sync()
{
    barrier_wait(context.barrier);
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
        CopyMemory(context.shared_buffer, lane_local_storage, broadcast_size);
    }
    lane_sync();
    
    if (lane_idx() != broadcast_src_lane_idx)
    {
        CopyMemory(lane_local_storage, context.shared_buffer, broadcast_size);
    }
    lane_sync();
}
