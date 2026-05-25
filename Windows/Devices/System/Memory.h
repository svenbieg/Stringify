//==========
// Memory.h
//==========

#pragma once


//=======
// Using
//=======

#include "TypeHelper.h"
#include <stdlib.h>


//===========
// Namespace
//===========

namespace Devices {
	namespace System {


//========
// Memory
//========

class Memory
{
public:
	// Common
	static inline VOID* Allocate(SIZE_T Size)
		{
		return malloc(Size);
		}
	static inline VOID* AllocateAligned(SIZE_T Size, SIZE_T Align)
		{
		throw NotImplementedException();
		}
	static inline VOID Free(VOID* Buffer)
		{
		free(Buffer);
		}
};

}}
