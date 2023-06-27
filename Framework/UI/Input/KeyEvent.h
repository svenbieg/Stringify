//============
// KeyEvent.h
//============

#pragma once


//=======
// Using
//=======

#include "VirtualKey.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Input {


//================
// Key-Event-Type
//================

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
	// Con-/Destructors
	KeyEventArgs(VirtualKey Key, TCHAR Char=0);

	// Common
	TCHAR Char;
	BOOL Handled;
	VirtualKey Key;
};

}}