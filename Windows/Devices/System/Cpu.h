//=======
// Cpu.h
//=======

#pragma once


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