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
	static inline VOID DebugBreak() { ::DebugBreak(); }
	static inline VOID Print(LPCSTR Text) { OutputDebugStringA(Text); }
	template <class... _args_t> static inline VOID Print(LPCSTR Format, _args_t... Arguments)
		{
		auto text=String::Create(Format, Arguments...);
		OutputDebugString(text->Begin());
		}
	#else
	static inline VOID DebugBreak() {}
	template <class... _args_t> static inline VOID Print(_args_t... Arguments) {}
	#endif
	template <class _exc_t> VOID Throw()
		{
		DebugBreak();
		throw _exc_t();
		}
	static inline VOID ThrowIfFailed(HRESULT Status)
		{
		if(FAILED(Status))
			{
			DebugBreak();
			throw AbortException();
			}
		}
	template <class _value_t> static inline VOID ThrowIfNull(_value_t Value)
		{
		if(Value==0)
			{
			DebugBreak();
			throw AbortException();
			}
		}
};
