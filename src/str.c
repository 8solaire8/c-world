#include <windows.h>

#include "defs.h"
#include "str.h"
#include "arena.h"

String8 
string8_new(char* c_string) 
{
    String8 res = {0};
    U64 size = cstr_len(c_string);
    res.data = c_string;
    res.size = size;
    return res;
}

char*
string8_to_char(Arena* arena, String8 str)
{
    char* res = push_array_zero(arena, char, str.size + 1);
    CopyMemory(res, str.data, str.size);
    return res;
}
wchar*
string8_to_wchar(Arena* arena, String8 str)
{
    
}

S32 
string8_compare(String8 a, String8 b) 
{
    U64 min_size = MIN(a.size, b.size);
    
    for (U64 i = 0; i < min_size; ++i) 
    {
        if (a.data[i] != b.data[i]) return (S32)a.data[i] - (S32)b.data[i];
    }
    
    if (a.size < b.size) return -(S32)(b.size - a.size);
    if (a.size > b.size) return  (S32)(a.size - b.size);
    return 0;
}


wchar* 
string16_to_wchar(Arena* arena, String16 str)
{
    wchar* res = push_array_zero(arena, wchar, str.size + 1);
    CopyMemory(res, str.data, str.size * sizeof(wchar));
    return res;
}

String8
string16_to_string8(Arena* arena, String16 src)
{
    ArenaTemp temp = get_scratch();
    String8 res = {0};
    wchar* wide = string16_to_wchar(temp.arena, src);
    S32 bytes = WideCharToMultiByte(CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
    
    U64 arena_pos = arena_get_pos(arena);
    res.data = push_array(arena, char, bytes);
    res.size = bytes;
    if (WideCharToMultiByte(CP_UTF8, 0, wide, -1, res.data, bytes, NULL, NULL) == 0) 
    {
        arena_set_pos(arena, arena_pos);
        res.data = NULL;
        res.size = 0;
    }
    release_scratch(temp);
    return res;
}

String16
string8_to_string16(Arena* arena, String8 src)
{
    String16 res = {0};
    
    ArenaTemp temp = get_scratch();
    
    char* utf8 = string8_to_char(temp.arena, src);
    
    U32 char_len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                       utf8, -1, NULL, 0);
    U64 arena_pos = arena_get_pos(arena);
    res.data = push_array_zero(arena, wchar, char_len);
    res.size = char_len;
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                            utf8, -1, res.data, char_len) == 0) 
    {
        arena_set_pos(arena, arena_pos);
        res.data = NULL;
        res.size = 0;
    }
    release_scratch(temp);
    return res;
}

char*
string16_to_char(Arena* arena, String16 str)
{
    
}



U64 
cstr_len(const char* c_str)
{
    U64 length = 0;
    while(c_str[length] != '\0')
    {
        length++;
    }
    return length;
}

bool 
is_ascii(char c)
{
    return c>= 0x20 && c <= 0x7E;
}
