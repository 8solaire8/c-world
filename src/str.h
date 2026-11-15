#ifndef STR_H
#define STR_H

#include "types.h"

struct String8 
{
    U8*   data;
    U64   size;
};

struct String16
{
    U16* data;
    U64 size;
};

String8 string8_new(char* c);
char*   string8_to_char(Arena* arena, String8 str);
wchar*  string8_to_wchar(Arena* arena, String8 str);
S32     string8_compare(String8 str1, String8 str2);

String16 string16_new(wchar* c);
wchar*   string16_to_wchar(Arena* arena, String16 str);
char*    string16_to_char(Arena* arena, String16 str);

String8 string16_to_string8(Arena* arena, String16 src);
String16 string8_to_string16(Arena* arena, String8 src);


U64 cstr_len(const char* c_str);
bool is_ascii(char c);
#endif
