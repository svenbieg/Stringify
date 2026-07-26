//============
// KeyEvent.h
//============

#pragma once


//=======
// Using
//=======

#include "UI/Input/VirtualKey.h"
#include "Object.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Input {


//=======
// Flags
//=======

enum class KeyEventFlags
{
None=0,
Alt=1,
Ctrl=2,
Shift=4
};


//======
// Type
//======

enum class KeyEventType
{
KeyDown,
KeyUp
};


//================
// Key-Event-Args
//================

class KeyEventArgs: public Object
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<KeyEventArgs> Create(VirtualKey Key, TCHAR Char=0)
		{
		return Object::Create<KeyEventArgs>(Key, Char);
		}

	// Common
	TCHAR Char;
	KeyEventFlags Flags;
	BOOL Handled;
	VirtualKey Key;

private:
	// Con-/Destructors
	KeyEventArgs(VirtualKey Key, TCHAR Char):
		Char(Char),
		Flags(KeyEventFlags::None),
		Handled(false),
		Key(Key)
		{}
};

}}