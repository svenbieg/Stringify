//=======
// Cpu.h
//=======

#pragma once


//=======
// Using
//=======

#include "TypeHelper.h"
#include <bit>


//===========
// Namespace
//===========

namespace Devices {
	namespace System {


//=====
// Cpu
//=====

class Cpu
{
public:
	// Common
	static inline UINT CountTrailingZeros(UINT Value)
		{
		return std::countr_zero(Value);
		}
	static inline UINT InterlockedDecrement(volatile UINT* Value)
		{
		return _InterlockedDecrement(Value);
		}
	static inline UINT InterlockedIncrement(volatile UINT* Value)
		{
		return _InterlockedIncrement(Value);
		}
};

}}