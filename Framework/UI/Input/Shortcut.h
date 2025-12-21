//============
// Shortcut.h
//============

#pragma once


//=======
// Using
//=======

#include "UI/Input/VirtualKey.h"
#include "StringClass.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Input {


//=======
// Flags
//=======

enum class ShortcutFlags: UINT
{
None=0,
Ctrl=0x100,
Shift=0x200,
Alt=0x400
};


//========
// Common
//========

UINT ShortcutFromString(Handle<String> Text);

}}