//==========
// Dialog.h
//==========

#pragma once


//=======
// Using
//=======

#include "Desktop/Overlapped.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Dialog
//========

class Dialog: public Overlapped
{
public:
	// Con-/Destructors
	Dialog(Window* Parent);

	// Common
	VOID Create(LPCTSTR Class=TEXT("Dialog"));
};

}}