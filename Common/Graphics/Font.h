//========
// Font.h
//========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {


//======
// Font
//======

class Font: public Object
{
public:
	// Common
	virtual UINT GetSize()const=0;

protected:
	// Con-/Destructors
	Font() {}
};

}