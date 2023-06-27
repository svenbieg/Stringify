//================
// MemoryHelper.h
//================

#pragma once


//========
// Macros
//========

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a)/sizeof((a)[0]))
#endif

#define MIN(a, b) ((a)<(b)? a: b)
#define MAX(a, b) ((a)>(b)? a: b)

#define OFFSETOF(type, element) ((SIZE_T)&(((type*)nullptr)->element))

#ifndef HIWORD
#define HIWORD(a) ((WORD)((a)>>16))
#endif

#ifndef LOWORD
#define LOWORD(a) ((WORD)((a)&0xFFFF))
#endif

#define HILONG(a) ((DWORD)((a)>>32))
#define LOLONG(a) ((DWORD)((a)&0xFFFFFFFF))

#ifndef MAKELONG
#define MAKELONG(lo, hi) ((((UINT)(hi))<<16)|(lo))
#endif

#define MAKELONGLONG(lo, hi) ((((UINT64)(hi))<<32)|(lo))


//===========
// Alignment
//===========

#define ALIGN(a) __declspec(align(a))

template <class _size_t>
inline _size_t BlockAlign(_size_t Size, UINT BlockSize)
{
return ((Size+BlockSize-1)/BlockSize)*BlockSize;
}


//===============
// Little Endian
//===============

template <class _value_t> _value_t Reverse(_value_t Value)
{
auto pvalue=(BYTE*)&Value;
_value_t retval=0;
auto pretval=(BYTE*)&retval;
for(UINT u=0; u<sizeof(_value_t); u++)
	pretval[u]=pvalue[sizeof(_value_t)-u-1];
return retval;
}

inline VOID ReverseCopy(VOID* Destination, VOID const* Source, SIZE_T Size)
{
auto dst=(BYTE*)Destination;
auto src=(BYTE const*)Source;
for(SIZE_T u=0; u<Size; u++)
	dst[Size-u-1]=src[u];
}
