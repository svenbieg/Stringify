//===========
// Point.cpp
//===========

#include "Graphics/Point.h"


//=======
// Using
//=======

#include "Graphics/Rect.h"


//===========
// Namespace
//===========

namespace Graphics {


//========
// Common
//========

BOOL POINT::Inside(RECT const& rc)const
{
if(Left<rc.Left)
	return false;
if(Top<rc.Top)
	return false;
if(Left>=rc.Right)
	return false;
if(Top>=rc.Bottom)
	return false;
return true;
}

}