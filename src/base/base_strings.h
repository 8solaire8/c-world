#ifndef BASE_STRINGS_H
#define BASE_STRINGS_H

typedef struct String8 String8;
struct String8 
{
    U8 *data;
    U64 size;
};

typedef struct String16 String16;
struct String16
{
    U16 *data;
    U64  size;
};

typedef struct UnicodeDecode UnicodeDecode;
struct UnicodeDecode
{
    U32 inc;
    U32 codepoint;
};

#define str8_lit(S)  str8((U8*)(S), sizeof(S) - 1)
#define str8_lit_comp(S) {(U8*)(S), sizeof(S) - 1,}
#define str8_lit_cstr(S) str8((U8*)(S), sizeof(S))
#define str8_varg(S) (int)((S).size), ((S).data)

internal String8  str8(U8 *str, U64 size);
internal String8  str8_range(U8 *first, U8 *one_past_last);
internal String8  str8_zero();
internal String8  str8_cstring(char *c);
internal String8  str8_copy(Arena *arena, String8 s);
internal char    *str8_to_char(Arena *arena, String8 str);
internal wchar   *str8_to_wchar(Arena *arena, String8 str);
internal S32      str8_compare(String8 str1, String8 str2);
internal String16 str8_to_16(Arena *arena, String8 src);



internal String16 str16(U16 *str, U64 size);
internal String16 str16_range(U16 *first, U16 *one_past_last);
internal String16 str16_zero();
internal String16 str16_cstring(wchar *c);
internal wchar   *str16_to_wchar(Arena *arena, String16 str);
internal char    *str16_to_char(Arena *arena, String16 str);
internal S32      str16_compare(String16 str1, String16 str2);
internal String8  str16_to_8(Arena *arena, String16 src);

internal U64 cstr8_len(U8 *c_str);
internal U64 cstr16_len(U16 *c_str);
internal bool is_ascii(char c);
#endif
