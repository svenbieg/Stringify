//============
// ComboBox.h
//============

#pragma once


//=======
// Using
//=======

#include "Grid.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//===========
// Combo-Box
//===========

class ComboBox: public Grid
{
public:
	// Con-/Destructors
	ComboBox(Window* Parent=nullptr);
};

}}