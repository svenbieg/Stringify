//=========
// Brush.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/Color.h"
#include "Event.h"


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
	Event<Brush> Changed;
	Event<Brush> Destroyed;
	inline COLOR GetColor()const { return m_Color; }
	VOID SetColor(COLOR Color);

protected:
	// Common
	UINT Release()override;

private:
	// Con-/Destructors
	Brush(COLOR Color): m_Color(Color) {}

	// Common
	COLOR m_Color;
};

}