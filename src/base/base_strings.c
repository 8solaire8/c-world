read_only global U8 utf8_class[32] =
{
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,2,2,2,2,3,3,4,5,
};

internal UnicodeDecode
utf8_decode(U8 *str, U64 max)
{
    UnicodeDecode result = {1, max_U32};
    U8 byte = str[0];
    U8 byte_class = utf8_class[byte >> 3];
    switch(byte_class)
    {
        case 1:
        {
            result.codepoint = byte;
        }break;
        case 2:
        {
            if(1 < max)
            {
                U8 cont_byte = str[1];
                if(utf8_class[cont_byte >> 3] == 0)
                {
                    result.codepoint = (byte & bitmask5) << 6;
                    result.codepoint |=  (cont_byte & bitmask6);
                    result.inc = 2;
                }
            }
        }break;
        case 3:
        {
            if(2 < max)
            {
                U8 cont_byte[2] = {str[1], str[2]};
                if(utf8_class[cont_byte[0] >> 3] == 0 &&
                   utf8_class[cont_byte[1] >> 3] == 0)
                {
                    result.codepoint = (byte & bitmask4) << 12;
                    result.codepoint |= ((cont_byte[0] & bitmask6) << 6);
                    result.codepoint |=  (cont_byte[1] & bitmask6);
                    result.inc = 3;
                }
            }
        }break;
        case 4:
        {
            if(3 < max)
            {
                U8 cont_byte[3] = {str[1], str[2], str[3]};
                if(utf8_class[cont_byte[0] >> 3] == 0 &&
                   utf8_class[cont_byte[1] >> 3] == 0 &&
                   utf8_class[cont_byte[2] >> 3] == 0)
                {
                    result.codepoint = (byte & bitmask3) << 18;
                    result.codepoint |= ((cont_byte[0] & bitmask6) << 12);
                    result.codepoint |= ((cont_byte[1] & bitmask6) <<  6);
                    result.codepoint |=  (cont_byte[2] & bitmask6);
                    result.inc = 4;
                }
            }
        }
    }
    return result;
}

internal UnicodeDecode
utf16_decode(U16 *str, U64 max)
{
    UnicodeDecode result = {1, max_U32};
    result.codepoint = str[0];
    result.inc = 1;
    if(max > 1 && 0xD800 <= str[0] && str[0] < 0xDC00 && 0xDC00 <= str[1] && str[1] < 0xE000)
    {
        result.codepoint = ((str[0] - 0xD800) << 10) | ((str[1] - 0xDC00) + 0x10000);
        result.inc = 2;
    }
    return result;
}

internal U32
utf8_encode(U8 *str, U32 codepoint)
{
    U32 inc = 0;
    if(codepoint <= 0x7F)
    {
        str[0] = (U8)codepoint;
        inc = 1;
    }
    else if(codepoint <= 0x7FF)
    {
        str[0] = (bitmask2 << 6) | ((codepoint >> 6) & bitmask5);
        str[1] = bit8 | (codepoint & bitmask6);
        inc = 2;
    }
    else if(codepoint <= 0xFFFF)
    {
        str[0] = (bitmask3 << 5) | ((codepoint >> 12) & bitmask4);
        str[1] = bit8 | ((codepoint >> 6) & bitmask6);
        str[2] = bit8 | ( codepoint       & bitmask6);
        inc = 3;
    }
    else if(codepoint <= 0x10FFFF)
    {
        str[0] = (bitmask4 << 4) | ((codepoint >> 18) & bitmask3);
        str[1] = bit8 | ((codepoint >> 12) & bitmask6);
        str[2] = bit8 | ((codepoint >>  6) & bitmask6);
        str[3] = bit8 | ( codepoint        & bitmask6);
        inc = 4;
    }
    else
    {
        str[0] = '?';
        inc = 1;
    }
    return inc;
}

internal U32
utf16_encode(U16 *str, U32 codepoint)
{
    U32 inc = 1;
    if(codepoint == max_U32)
    {
        str[0] = (U16)'?';
    }
    else if(codepoint < 0x10000)
    {
        str[0] = (U16)codepoint;
    }
    else
    {
        U32 v = codepoint - 0x10000;
        str[0] = safe_cast_u16(0xD800 + (v >> 10));
        str[1] = safe_cast_u16(0xDC00 + (v & bitmask10));
        inc = 2;
    }
    return inc;
}

internal String8
str8(U8 *str, U64 size)
{
    String8 result = {str, size};
    return result;
}

internal String8 
str8_zero()
{
    String8 result = {0};
    return result;
}
internal String8 
str8_cstring(char* c_string) 
{
    String8 res = {0};
    U64 size = cstr_len(c_string);
    res.data = c_string;
    res.size = size;
    return res;
}

internal String8
str8_copy(Arena*a, String8 str)
{
    
}

internal char*
str8_to_char(Arena* arena, String8 str)
{
    char* res = push_array_zero(arena, char, str.size + 1);
    CopyMemory(res, str.data, str.size);
    return res;
}

internal wchar*
str8_to_wchar(Arena* arena, String8 str)
{
    assert(1);
}

internal S32 
str8_compare(String8 a, String8 b) 
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

internal String16
str16(U16 *str, U64 size)
{
    String16 result = {str, size};
    return result;
}

internal String16
str16_zero(void)
{
    String16 result = {0};
    return result;
}

internal wchar* 
str16_to_wchar(Arena* arena, String16 str)
{
    wchar* res = push_array_zero(arena, wchar, str.size + 1);
    CopyMemory(res, str.data, str.size * sizeof(wchar));
    return res;
}

internal String8
str8_from_16(Arena *arena, String16 in)
{
    String8 result = str8_zero();
    if(in.size)
    {
        U64 cap = in.size*3;
        U8 *str = push_array_no_zero(arena, U8, cap + 1);
        U16 *ptr = in.str;
        U16 *opl = ptr + in.size;
        U64 size = 0;
        UnicodeDecode consume;
        for(;ptr < opl; ptr += consume.inc)
        {
            consume = utf16_decode(ptr, opl - ptr);
            size += utf8_encode(str + size, consume.codepoint);
        }
        str[size] = 0;
        arena_pop(arena, (cap - size));
        result = str8(str, size);
    }
    return result;
}

internal String16
str16_from_8(Arena *arena, String8 in)
{
    String16 result = str16_zero();
    if(in.size)
    {
        U64 cap = in.size*2;
        U16 *str = push_array_no_zero(arena, U16, cap + 1);
        U8 *ptr = in.str;
        U8 *opl = ptr + in.size;
        U64 size = 0;
        UnicodeDecode consume;
        for(;ptr < opl; ptr += consume.inc)
        {
            consume = utf8_decode(ptr, opl - ptr);
            size += utf16_encode(str + size, consume.codepoint);
        }
        str[size] = 0;
        arena_pop(arena, (cap - size)*2);
        result = str16(str, size);
    }
    return result;
}

internal char*
str16_to_char(Arena* arena, String16 str)
{
    
}


internal U64 
cstr8_len(U8* c_str)
{
    U64 length = 0;
    while(c_str[length] != '\0')
    {
        length++;
    }
    return length;
}

internal bool 
is_ascii(char c)
{
    return c>= 0x20 && c <= 0x7E;
}
