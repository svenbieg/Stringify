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
};

}