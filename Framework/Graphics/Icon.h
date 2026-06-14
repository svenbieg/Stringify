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
	using IconMap=Collections::map<UINT, Handle<Bitmap>>;
	using ResourceMap=Collections::map<ICON const*, Handle<Icon>>;

	// Friends
	friend Object;

	// Con-/Destructors
	IconMap Bitmaps;
	static Handle<Icon> Create(ICON const* Resource);
	static inline Handle<Icon> Create(IconMap const& Bitmaps)
		{
		return Object::Create<Icon>(Bitmaps);
		}

	// Common
	Handle<Bitmap> GetBitmap(UINT Size);

protected:
	// Con-/Destructors
	Icon(ICON const* Resource);
	Icon(IconMap const& Bitmaps);

	// Common
	ICON const* GetIcon(UINT Size);
	ICON const* m_Icons;
	static ResourceMap s_Resources;
};

}