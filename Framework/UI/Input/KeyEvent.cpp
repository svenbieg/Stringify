//==============
// KeyEvent.cpp
//==============

#include "pch.h"


//========
// Common
//========

#include "KeyEvent.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Input {


//==================
// Con-/Destructors
//==================

KeyEventArgs::KeyEventArgs(VirtualKey key, TCHAR c):
Char(c),
Handled(false),
Key(key)
{}

}}