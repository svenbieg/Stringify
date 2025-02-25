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


//========
// Common
//========

#ifdef _DEBUG

inline VOID DebugPrint(LPCSTR Text) { OutputDebugStringA(Text); }

template <class... _args_t> VOID DebugPrint(LPCSTR Format, _args_t... Arguments)
{
auto text=String::Create(Format, Arguments...);
OutputDebugString(text->Begin());
}

#else

#define DebugPrint(...)

#endif

inline VOID ThrowIfFailed(HRESULT Status) { if(FAILED(Status))throw AbortException(); }
