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
	// Common
	virtual COLOR GetColor()const=0;

protected:
	// Con-/Destructors
	Brush() {}
};

}