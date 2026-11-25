internal U16
safe_cast_u16(U32 x)
{
    AssertAlways(x <= max_U16);
    U16 result = (U16)x;
    return result;
}

internal U32
safe_cast_u32(U64 x)
{
    AssertAlways(x <= max_U32);
    U32 result = (U32)x;
    return result;
}

internal S32
safe_cast_s32(S64 x)
{
    AssertAlways(x <= max_S32);
    S32 result = (S32)x;
    return result;
}