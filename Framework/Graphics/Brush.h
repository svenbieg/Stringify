//=========
// Brush.h
//=========

#pragma once


//=======
// Using
//=======

#include "Color.h"


//===========
// Namespace
//===========

namespace Graphics {


//=======
// Brush
//=======

class Brush: public Object
{
public:
	// Con-/Destructors
	Brush(COLOR Color): m_Color(Color) {}

	// Common
	inline COLOR GetColor()const { return m_Color; }

private:
	// Common
	COLOR m_Color;
};

}