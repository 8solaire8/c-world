#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

internal void* pages_reserve(U64 size);

internal void pages_release(void* ptr, U64 size);

internal void* pages_commit(void* ptr, U64 current_commited_bytes, U64 new_bytes_to_commit);

#endif //BASE_MEMORY_H
