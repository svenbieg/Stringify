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
	static inline Handle<Brush> Create(COLOR Color) { return new Brush(Color); }

	// Common
	inline COLOR GetColor()const { return m_Color; }
	inline VOID SetColor(COLOR Color) { m_Color=Color; }

private:
	// Con-/Destructors
	Brush(COLOR Color): m_Color(Color) {}

	// Common
	COLOR m_Color;
};

}