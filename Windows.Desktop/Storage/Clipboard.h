//=============
// Clipboard.h
//=============

#pragma once


//=======
// Using
//=======

#include "Global.h"
#include "StringClass.h"


//===========
// Namespace
//===========

namespace Storage {


//===========
// Clipboard
//===========

class Clipboard: public Global<Clipboard>
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<Clipboard> Create() { return Global::Create(); }

	// Common
	VOID Copy(Handle<String> Text);
	Handle<String> GetText();
	BOOL HasText();

private:
	// Con-/Destructors
	Clipboard() {}
};

}