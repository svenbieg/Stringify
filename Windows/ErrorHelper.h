//===============
// ErrorHelper.h
//===============

#pragma once


//=======
// Using
//=======

#include <assert.h>
#include "MemoryHelper.h"
#include "StringClass.h"
#include "TaskHelper.h"


//========
// Common
//========

#ifdef _DEBUG

inline VOID DebugPrint(LPCSTR Text) { OutputDebugStringA(Text); }

template <class... _args_t> VOID DebugPrint(LPCSTR Format, _args_t... Arguments)
{
Handle<String> text=new String(Format, Arguments...);
OutputDebugString(text->Begin());
}

inline VOID ThrowIfNotMainThread()
{
if(GetCurrentTask())
	throw E_NOINTERFACE;
}

#else

#define DebugPrint(...)
#define ThrowIfNotMainThread(...)

#endif

inline VOID ThrowIfFailed(HRESULT Status) { if(FAILED(Status))throw Status; }
