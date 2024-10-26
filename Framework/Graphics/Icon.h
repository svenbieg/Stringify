//========
// Icon.h
//========

#pragma once


//=======
// Using
//=======

#include "Resources/Icons/Icon.h"
#include "Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {


//======
// Icon
//======

class Icon: public Object
{
public:
	// Using
	using ICON=Resources::Icons::ICON;

	// Con-/Destructors
	Icon(ICON const* Icons=nullptr);

	// Common
	Handle<Bitmap> GetBitmap(UINT Size);

protected:
	// Common
	ICON const* GetIcon(UINT Size);
	ICON const* pIcons;
};

}