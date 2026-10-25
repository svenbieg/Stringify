//============
// Platform.h
//============

#pragma once


//=========
// Windows
//=========

#define _WINSOCKAPI_
#define SECURITY_WIN32

#include <Windows.h>

#undef min
#undef max

#undef VOID
#undef TEXT

#undef DrawText
#undef GetCurrentTime
#undef InterlockedDecrement
#undef InterlockedIncrement

#undef MessageBox
#ifdef _UNICODE
#define MessageBoxT MessageBoxW
#else
#define MessageBoxT MessageBoxA
#endif

#define WM_DISPATCH (WM_USER+1)

#define ICO_APP 1


//======================
// Forward-Declarations
//======================

#define __long long
#define __long_long long long

#ifdef _X86_
#define __addr_t unsigned long
#else
#define __addr_t unsigned long long
#endif

#define COPY_MAX 2
