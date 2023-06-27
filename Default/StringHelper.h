//================
// StringHelper.h
//================

#pragma once


//=======
// Using
//=======

#include "CharHelper.h"
#include "MemoryHelper.h"
#include "VariableArguments.h"


//========
// Common
//========

UINT StringCopy(LPSTR Destination, UINT Size, LPCSTR Source, UINT Length=0);
UINT StringCopy(LPSTR Destination, UINT Size, LPCWSTR Source, UINT Length=0);
UINT StringCopy(LPWSTR Destination, UINT Size, LPCSTR Source, UINT Length=0);
UINT StringCopy(LPWSTR Destination, UINT Size, LPCWSTR Source, UINT Length=0);
BOOL StringFind(LPCSTR String, LPCSTR Find, UINT* Position=nullptr, BOOL CaseSensitive=true);
BOOL StringFind(LPCSTR String, LPCWSTR Find, UINT* Position=nullptr, BOOL CaseSensitive=true);
BOOL StringFind(LPCWSTR String, LPCSTR Find, UINT* Position=nullptr, BOOL CaseSensitive=true);
BOOL StringFind(LPCWSTR String, LPCWSTR Find, UINT* Position=nullptr, BOOL CaseSensitive=true);
BOOL StringFindChar(LPCSTR String, CHAR Find, UINT* Position=nullptr, BOOL CaseSensitive=true);
BOOL StringFindChar(LPCWSTR String, CHAR Find, UINT* Position=nullptr, BOOL CaseSensitive=true);
BOOL StringFindChars(LPCSTR String, LPCSTR Find, UINT* Position=nullptr, BOOL CaseSensitive=true);
BOOL StringFindChars(LPCWSTR String, LPCSTR Find, UINT* Position=nullptr, BOOL CaseSensitive=true);
inline BOOL StringIsSet(LPCSTR String) { return String&&String[0]; }
inline BOOL StringIsSet(LPCWSTR String) { return String&&String[0]; }
UINT StringLength(LPCSTR String);
UINT StringLength(LPCWSTR String);
UINT StringLength(LPCSTR String, UINT Max);
UINT StringLength(LPCWSTR String, UINT Max);


//==============
// Modification
//==============

UINT StringAppend(LPSTR Destination, UINT Size, LPCSTR String, LPCSTR Append);
UINT StringInsert(LPSTR Destination, UINT Size, LPCSTR String, UINT Position, LPCSTR Insert);
UINT StringLowerCase(LPSTR Destination, UINT Size, LPCSTR String);
UINT StringReplace(LPSTR Destination, UINT Size, LPCSTR String, LPCSTR Find, LPCSTR Replace, BOOL CaseSensitive=true, BOOL Repeat=false);
UINT StringReplace(LPWSTR Destination, UINT Size, LPCWSTR String, LPCSTR Find, LPCSTR Replace, BOOL CaseSensitive=true, BOOL Repeat=false);
UINT StringUpperCase(LPSTR Destination, UINT Size, LPCSTR String);


//============
// Formatting
//============

UINT StringVPrint(LPSTR Buffer, UINT Size, LPCSTR Format, VariableArguments& Arguments);
UINT StringVPrint(LPWSTR Buffer, UINT Size, LPCSTR Format, VariableArguments& Arguments);

template <class _char_t, class _fmt_t, class... _args_t>
inline UINT StringPrint(_char_t* String, UINT Size, _fmt_t const* Format, _args_t... Arguments)
{
UnknownClass args[]={ Arguments... };
VariableArguments vargs(args, ARRAYSIZE(args));
return StringVPrint(String, Size, Format, vargs);
}


//==========
// Scanning
//==========

UINT StringVScan(LPCSTR String, LPCSTR Format, VariableArguments& Arguments);
UINT StringVScan(LPCWSTR String, LPCSTR Format, VariableArguments& Arguments);

template <class _char_t, class... _args_t>
inline UINT StringScan(_char_t const* String, LPCSTR Format, _args_t... Arguments)
{
UnknownClass args[]={ Arguments... };
VariableArguments vargs(args, ARRAYSIZE(args));
return StringVScan(String, Format, vargs);
}


//============
// Comparison
//============

INT StringCompare(LPCSTR Value1, LPCSTR Value2, UINT Length=0, BOOL CaseSensitive=true);
INT StringCompare(LPCSTR Value1, LPCWSTR Value2, UINT Length=0, BOOL CaseSensitive=true);
INT StringCompare(LPCWSTR Value1, LPCSTR Value2, UINT Length=0, BOOL CaseSensitive=true);
INT StringCompare(LPCWSTR Value1, LPCWSTR Value2, UINT Length=0, BOOL CaseSensitive=true);
