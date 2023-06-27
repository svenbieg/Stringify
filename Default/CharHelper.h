//==============
// CharHelper.h
//==============

#pragma once


//============
// Characters
//============

#ifdef _UNICODE
namespace Chars
{
constexpr WCHAR Average=L'\x00D8';
constexpr WCHAR Degree=L'\x00B0';
constexpr WCHAR Enumeration=L'\x2022';
}
#else
constexpr CHAR Average='\xD8';
constexpr CHAR Degree='\xB0';
constexpr CHAR Enumeration='\x95';
#endif


//============
// Conversion
//============

CHAR CharToAnsi(WCHAR Char);
CHAR CharToCapital(CHAR Char);
WCHAR CharToCapital(WCHAR Char);
BYTE CharToHex(CHAR Char);
BYTE CharToHex(WCHAR Char);
CHAR CharToSmall(CHAR Char);
WCHAR CharToSmall(WCHAR Char);
WCHAR CharToUnicode(CHAR Char);

template <class _convert_t, class _char_t> inline _convert_t CharToChar(_char_t Char)
{
return (_convert_t)Char;
}

template <> inline CHAR CharToChar(WCHAR Char)
{
return CharToAnsi(Char);
}

template <> inline WCHAR CharToChar(CHAR Char)
{
return CharToUnicode(Char);
}


//========
// Common
//========

BOOL CharIsAlpha(CHAR Char);
BOOL CharIsAlpha(WCHAR Char);
BOOL CharIsBinary(CHAR Char);
BOOL CharIsBinary(WCHAR Char);
BOOL CharIsBreak(CHAR Char);
BOOL CharIsBreak(WCHAR Char);
BOOL CharIsCapital(CHAR Char);
BOOL CharIsCapital(WCHAR Char);
BOOL CharIsHex(CHAR Char);
BOOL CharIsHex(WCHAR Char);
BOOL CharIsNumber(CHAR Char);
BOOL CharIsNumber(WCHAR Char);
BOOL CharIsPrintable(CHAR Char);
BOOL CharIsPrintable(WCHAR Char);
BOOL CharIsSmall(CHAR Char);
BOOL CharIsSmall(WCHAR Char);
BOOL CharIsSpecial(CHAR Char);
BOOL CharIsSpecial(WCHAR Char);


//============
// Comparison
//============

INT CharCompare(CHAR Char1, CHAR Char2, BOOL CaseSensitive=true);
INT CharCompare(WCHAR Char1, WCHAR Char2, BOOL CaseSensitive=true);
INT CharCompare(CHAR Char1, WCHAR Char2, BOOL CaseSensitive=true);
INT CharCompare(WCHAR Char1, CHAR Char2, BOOL CaseSensitive=true);

template <class _char1_t, class _char2_t> inline BOOL CharEqual(_char1_t Char1, _char2_t Char2)
{
CHAR c1=CharToChar<CHAR, _char1_t>(Char1);
CHAR c2=CharToChar<CHAR, _char2_t>(Char2);
return c1==c2;
}

BOOL CharEqual(CHAR Char1, CHAR Char2, BOOL CaseSensitive=true);
BOOL CharEqual(CHAR Char1, WCHAR Char2, BOOL CaseSensitive=true);
BOOL CharEqual(WCHAR Char1, CHAR Char2, BOOL CaseSensitive=true);
BOOL CharEqual(WCHAR Char1, WCHAR Char2, BOOL CaseSensitive=true);
