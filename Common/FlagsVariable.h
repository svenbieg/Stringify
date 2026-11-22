//=================
// FlagsVariable.h
//=================

#pragma once


//=======
// Using
//=======

#include "Collections/index.hpp"
#include "Variable.h"


//================
// Flags-Variable
//================

class FlagsVariable: public Variable
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<FlagsVariable> Create(Handle<String> Name) { return Object::Create<FlagsVariable>(Name); }

	// Access
	inline BOOL Get(Handle<String> Flag) { return m_Flags.contains(Flag); }
	inline Handle<String> GetName()const override { return m_Name; }
	Handle<String> ToString(LanguageCode Language=LanguageCode::None)override;
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Modification
	VOID Clear();
	VOID Clear(Handle<String> Flag);
	BOOL FromString(Handle<String> Value)override;
	SIZE_T ReadFromStream(InputStream* Stream)override;
	VOID Set(Handle<String> Flag);

private:
	// Con-/Destructors
	FlagsVariable(Handle<String> Name): m_Name(Name) {}

	// Common
	Collections::index<Handle<String>> m_Flags;
	Handle<String> m_Name;
};
