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

#undef INT_MAX
#undef INT_MIN
#undef SIZE_MAX
#undef UINT_MAX
#undef UINT64_MAX
#undef VOID
#undef WORD_MAX

#undef TEXT

#undef DrawText
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

#define __COPY_MAX 2
#define __FILE_SIZE_T unsigned long long int
#define __LONG long
#define __LONG_LONG long long

#ifdef _X86_
#define __SIZE_T unsigned long
#else
#define __SIZE_T unsigned long long
#endif
