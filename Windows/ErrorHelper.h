//===============
// ErrorHelper.h
//===============

#pragma once


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

#define ASSERT(Condition) if(!(Condition)){ DebugPrint("%s:%u ASSERT\n", (LPCSTR)__FILE__, (UINT)__LINE__); throw E_FAIL; }

#else

#define DebugPrint(...)
#define ASSERT(Condition)

#endif

inline VOID ThrowIfFailed(HRESULT Status) { if(FAILED(Status))throw Status; }
