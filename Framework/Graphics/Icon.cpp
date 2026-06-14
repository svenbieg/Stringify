//==========
// Icon.cpp
//==========

#include "Icon.h"


//=======
// Using
//=======

using namespace Resources::Icons;


//===========
// Namespace
//===========

namespace Graphics {


//==================
// Con-/Destructors
//==================

Handle<Icon> Icon::Create(ICON const* resource)
{
Handle<Icon> icon;
if(s_Resources.try_get(resource, &icon))
	return icon;
icon=Object::Create<Icon>(resource);
s_Resources.add(resource, icon);
return icon;
}


//========
// Common
//========

Handle<Bitmap> Icon::GetBitmap(UINT size)
{
Handle<Bitmap> bmp;
if(!Bitmaps.try_get(size, &bmp))
	{
	auto icon=GetIcon(size);
	if(!icon)
		return nullptr;
	size=icon->Size;
	if(!Bitmaps.try_get(size, &bmp))
		{
		bmp=Bitmap::Create(size, size, 32, icon->Buffer);
		Bitmaps.add(size, bmp);
		}
	}
return bmp;
}


//============================
// Con-/Destructors Protected
//============================

Icon::Icon(ICON const* resource):
m_Icons(resource)
{}

Icon::Icon(IconMap const& bitmaps):
Bitmaps(bitmaps),
m_Icons(nullptr)
{}


//==================
// Common Protected
//==================

ICON const* Icon::GetIcon(UINT size)
{
if(!m_Icons)
	return nullptr;
ICON const* prev=nullptr;
for(ICON const* icon=m_Icons; icon->Size>0; icon++)
	{
	if(icon->Size==size)
		return icon;
	prev=icon;
	if(icon->Size>size)
		break;
	}
return prev;
}

Icon::ResourceMap Icon::s_Resources;

}