//========
// Guid.h
//========

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//========
// Struct
//========

class GLOBAL_UNIQUE_ID
{
public:
	// Con-/Destructors
	GLOBAL_UNIQUE_ID() { ZeroMemory(m_Value, 16); }
	GLOBAL_UNIQUE_ID(GLOBAL_UNIQUE_ID const& Id) { CopyMemory(m_Value, Id.m_Value, 16); }
	GLOBAL_UNIQUE_ID(DWORD Data1, WORD Data2, WORD Data3, BYTE Data4, BYTE Data5, BYTE Data6, BYTE Data7, BYTE Data8, BYTE Data9, BYTE Data10, BYTE Data11);

	// Access
	Handle<String> ToString()const { return ToString(*this); }
	static Handle<String> ToString(GLOBAL_UNIQUE_ID const& Id);

	// Comparison
	BOOL operator==(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(m_Value, Id.m_Value, 16)==0; }
	BOOL operator>(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(m_Value, Id.m_Value, 16)>0; }
	BOOL operator>=(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(m_Value, Id.m_Value, 16)>=0; }
	BOOL operator<(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(m_Value, Id.m_Value, 16)<0; }
	BOOL operator<=(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(m_Value, Id.m_Value, 16)<=0; }

private:
	// Common
	BYTE m_Value[16];
};


//==========
// Variable
//==========

class Guid: public TypedVariable<GLOBAL_UNIQUE_ID>
{
public:
	// Con-/Destructors
	Guid(): Guid(nullptr) {}
	Guid(GLOBAL_UNIQUE_ID const& Value): Guid(nullptr, Value) {}
	Guid(DWORD Data1, WORD Data2, WORD Data3, BYTE Data4, BYTE Data5, BYTE Data6, BYTE Data7, BYTE Data8, BYTE Data9, BYTE Data10, BYTE Data11):
		Guid(nullptr, GLOBAL_UNIQUE_ID(Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Data9, Data10, Data11)) {}
	Guid(Handle<String> Name): Guid(Name, GLOBAL_UNIQUE_ID()) {}
	Guid(Handle<String> Name, GLOBAL_UNIQUE_ID const& Value): TypedVariable(Name, Value) {}
	Guid(Handle<String> Name, DWORD Data1, WORD Data2, WORD Data3, BYTE Data4, BYTE Data5, BYTE Data6, BYTE Data7, BYTE Data8, BYTE Data9, BYTE Data10, BYTE Data11):
		Guid(Name, GLOBAL_UNIQUE_ID(Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Data9, Data10, Data11)) {}

	// Common
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override;
};


//=============
// Guid-Handle
//=============

template <>
class Handle<Guid>: public ::Details::VariableHandle<Guid, GLOBAL_UNIQUE_ID>
{
public:
	// Using
	using _base_t=::Details::VariableHandle<Guid, GLOBAL_UNIQUE_ID>;
	using _base_t::_base_t;
};