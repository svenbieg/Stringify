//=========
// Popup.h
//=========

#pragma once


//=======
// Using
//=======

#include "Overlapped.h"


//===========
// Namespace
//===========

namespace UI {


//=======
// Popup
//=======

class Popup: public Overlapped
{
protected:
	// Con-/Destructors
	Popup(Window* Parent);

	// Common
	LRESULT HandleMessage(UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled)override;
};

}