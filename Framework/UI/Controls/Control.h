//===========
// Control.h
//===========

#pragma once


//=======
// Using
//=======

#include "UI/Window.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//=========
// Control
//=========

class Control: public Window
{
public:
	// Common
	RECT Margin;

protected:
	// Con-/Destructors
	Control(Window* Parent);
};

}}