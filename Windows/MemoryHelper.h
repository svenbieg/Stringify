//================
// MemoryHelper.h
//================

#pragma once


//=======
// Using
//=======

#include "TypeHelper.h"


//========
// Common
//========

constexpr UINT PAGE_SIZE=4096;

inline INT CompareMemory(VOID const* Buffer1, VOID const* Buffer2, SIZE_T Size)
{
return memcmp(Buffer1, Buffer2, Size);
}

#undef CopyMemory
inline VOID* CopyMemory(VOID* Destination, VOID const* Source, SIZE_T Size)
{
return memcpy(Destination, Source, Size);
}

#undef FillMemory
inline VOID* FillMemory(VOID* Destination, SIZE_T Size, INT Fill)
{
return memset(Destination, Fill, Size);
}

#undef ZeroMemory
inline VOID* ZeroMemory(VOID* Destination, SIZE_T Size)
{
return memset(Destination, 0, Size);
}
