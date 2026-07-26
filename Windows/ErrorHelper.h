//===============
// ErrorHelper.h
//===============

#pragma once


//=======
// Using
//=======

#include "MemoryHelper.h"
#include "StringClass.h"


//==============
// Error-Helper
//==============

class ErrorHelper
{
public:
	// Common
	#ifdef _DEBUG
	static inline VOID Print(LPCSTR Text) { OutputDebugStringA(Text); }
	template <class... _args_t> static inline VOID Print(LPCSTR Format, _args_t... Arguments)
		{
		auto text=String::Create(Format, Arguments...);
		OutputDebugString(text->Begin());
		}
	#else
	template <class... _args_t> static inline VOID Print(_args_t... Arguments) {}
	#endif
	static inline VOID ThrowIfFailed(HRESULT Status) { if(FAILED(Status))throw AbortException(); }
};
