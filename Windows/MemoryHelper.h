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

inline INT CompareMemory(VOID const* Buffer1, VOID const* Buffer2, SIZE_T Size)
{
return memcmp(Buffer1, Buffer2, Size);
}

inline VOID CopyMemory(VOID* Destination, VOID const* Source, SIZE_T Size)
{
memcpy(Destination, Source, Size);
}

inline VOID FillMemory(VOID* Destination, SIZE_T Size, UINT Value)
{
memset(Destination, Value, Size);
}

inline VOID MoveMemory(VOID* Destination, VOID const* Source, SIZE_T Size)
{
memmove(Destination, Source, Size);
}

inline VOID ZeroMemory(VOID* Destination, SIZE_T Size)
{
memset(Destination, 0, Size);
}
