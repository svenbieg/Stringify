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
	static inline VOID DebugPrint(LPCSTR Text) { OutputDebugStringA(Text); }
	template <class... _args_t> static inline VOID DebugPrint(LPCSTR Format, _args_t... Arguments)
		{
		auto text=String::Create(Format, Arguments...);
		OutputDebugString(text->Begin());
		}
	#else
	#define DebugPrint(...)
	#endif
	static inline VOID ThrowIfFailed(HRESULT Status) { if(FAILED(Status))throw AbortException(); }
};
