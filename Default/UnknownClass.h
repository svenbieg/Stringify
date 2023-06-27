//================
// UnknownClass.h
//================

#pragma once


//======================
// Forward-Declarations
//======================

class String;
template <class _obj_t> class Handle;


//==============
// Unknown Type
//==============

enum class UnknownType
{
None,
Int32,
Int32Ptr,
Int64,
Int64Ptr,
UInt32,
UInt32Ptr,
UInt64,
UInt64Ptr,
#ifndef _DRIVER
Float,
FloatPtr,
Double,
DoublePtr,
#endif
Char,
WChar,
String,
WString
};


//===============
// Unknown Class
//===============

class UnknownClass
{
public:
	// Con-/Destructors
	UnknownClass(UnknownClass const& Value);
	UnknownClass(INT Value);
	UnknownClass(INT* Value);
	UnknownClass(INT64 Value);
	UnknownClass(INT64* Value);
	UnknownClass(UINT Value);
	UnknownClass(UINT* Value);
	UnknownClass(UINT64 Value);
	UnknownClass(UINT64* Value);
	#ifndef _DRIVER
	UnknownClass(FLOAT Value);
	UnknownClass(FLOAT* Value);
	UnknownClass(DOUBLE Value);
	UnknownClass(DOUBLE* Value);
	#endif
	UnknownClass(CHAR Value);
	UnknownClass(WCHAR Value);
	UnknownClass(LPSTR Value);
	UnknownClass(LPCSTR Value);
	UnknownClass(LPWSTR Value);
	UnknownClass(LPCWSTR Value);
	UnknownClass(Handle<String> Value);

	// Access
	BOOL Get(INT& Value);
	BOOL Get(INT*& Value);
	BOOL Get(INT64& Value);
	BOOL Get(INT64*& Value);
	BOOL Get(UINT& Value);
	BOOL Get(UINT*& Value);
	BOOL Get(UINT64& Value);
	BOOL Get(UINT64*& Value);
	#ifndef _DRIVER
	BOOL Get(FLOAT& Value);
	BOOL Get(FLOAT*& Value);
	BOOL Get(DOUBLE& Value);
	BOOL Get(DOUBLE*& Value);
	#endif
	BOOL Get(CHAR& Value);
	BOOL Get(WCHAR& Value);
	BOOL Get(LPSTR& Value);
	BOOL Get(LPCSTR& Value);
	BOOL Get(LPWSTR& Value);
	BOOL Get(LPCWSTR& Value);

private:
	// Common
	alignas(8) BYTE pBuffer[8];
	UnknownType uType;
};
