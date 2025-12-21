//==================
// MemoryHelper.cpp
//==================

#include "TypeHelper.h"


//=======
// Using
//=======

#include "Exception.h"
#include <stdlib.h>


//========
// Common
//========

VOID* Allocate(SIZE_T size)
{
return malloc(size);
}

VOID* AllocateAligned(SIZE_T size, SIZE_T align)
{
throw NotImplementedException();
}

VOID Free(VOID* buf)
{
free(buf);
}
