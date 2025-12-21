//==============
// KeyEvent.cpp
//==============

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
Flags(KeyEventFlags::None),
Handled(false),
Key(key)
{}

}}