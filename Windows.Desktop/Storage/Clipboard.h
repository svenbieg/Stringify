//=============
// Clipboard.h
//=============

#pragma once


//===========
// Namespace
//===========

namespace Storage {


//===========
// Clipboard
//===========

class Clipboard: public Object
{
public:
	// Common
	VOID Copy(Handle<String> Text);
	Handle<String> GetText();
	BOOL HasText();
	static Handle<Clipboard> Open();

private:
	// Con-/Destructors
	Clipboard();

	// Common
	static Handle<Clipboard> hCurrent;
};

}