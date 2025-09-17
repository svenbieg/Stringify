//========
// Icon.h
//========

#pragma once


//=======
// Using
//=======

#include "Collections/map.hpp"
#include "Resources/Icons/Icon.h"
#include "Graphics/Bitmap.h"


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
	static inline Handle<Icon> Create(ICON const* Resource) { return new Icon(Resource); }

	// Common
	Handle<Bitmap> GetBitmap(UINT Size);

protected:
	// Con-/Destructors
	Icon(ICON const* Resource);

	// Common
	ICON const* GetIcon(UINT Size);
	Collections::map<UINT, Handle<Bitmap>> m_Bitmaps;
	ICON const* m_Icons;
};

}