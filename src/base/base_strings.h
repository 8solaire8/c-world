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

String8 str8(char* c);
String8 str8_copy(Arena *arena, String8 s);
char*   str8_to_char(Arena* arena, String8 str);
wchar*  str8_to_wchar(Arena* arena, String8 str);
S32     str8_compare(String8 str1, String8 str2);


String16 str16_new(wchar* c);
wchar*   str16_to_wchar(Arena* arena, String16 str);
char*    str16_to_char(Arena* arena, String16 str);

String8 str16_to_8(Arena* arena, String16 src);
String16 str8_to_16(Arena* arena, String8 src);


U64 cstr_len(const char* c_str);
bool is_ascii(char c);
#endif
