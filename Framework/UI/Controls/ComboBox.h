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
	static inline Handle<ComboBox> Create(Window* Parent) { return new ComboBox(Parent); }

private:
	// Con-/Destructors
	ComboBox(Window* Parent);
};

}}