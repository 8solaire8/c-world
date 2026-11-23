#ifndef BASE_STRINGS_H
#define BASE_STRINGS_H

typedef struct String8 String8;
struct String8 
{
    U8*   data;
    U64   size;
};

typedef struct String16 String16;
struct String16
{
    U16* data;
    U64 size;
};

typedef struct UnicodeDecode UnicodeDecode;
struct UnicodeDecode
{
    U32 inc;
    U32 codepoint;
};


internal String8 str8(U8 *str, U64 size);
internal String8 str8_zero();
internal String8 str8_cstring(char* c);
internal String8 str8_copy(Arena *arena, String8 s);
internal char*   str8_to_char(Arena* arena, String8 str);
internal wchar*  str8_to_wchar(Arena* arena, String8 str);
internal S32     str8_compare(String8 str1, String8 str2);



internal String16 str16(U16 *str, U64 size);
internal String16 str16_zero();
internal String16 str16_cstring(wchar* c);
internal wchar*   str16_to_wchar(Arena* arena, String16 str);
internal char*    str16_to_char(Arena* arena, String16 str);

internal String8 str16_to_8(Arena* arena, String16 src);
internal String16 str8_to_16(Arena* arena, String8 src);


internal U64 cstr8_len(U8* c_str);
internal bool is_ascii(char c);
#endif
