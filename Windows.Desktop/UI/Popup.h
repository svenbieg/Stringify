//=========
// Popup.h
//=========

#pragma once


//=======
// Using
//=======

#include "UI/Overlapped.h"


//===========
// Namespace
//===========

namespace UI {


//=======
// Popup
//=======

class Popup: public Overlapped
{
public:
	// Con-/Destructors
	static inline Handle<Popup> Create(Window* Content) { return new Popup(Content); }

protected:
	// Con-/Destructors
	Popup(Window* Content);
};

}