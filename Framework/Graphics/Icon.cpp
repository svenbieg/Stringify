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


//========
// Common
//========

Handle<Bitmap> Icon::GetBitmap(UINT size)
{
auto icon=GetIcon(size);
if(!icon)
	return nullptr;
size=icon->Size;
auto bmp=m_Bitmaps.get(size);
if(bmp)
	return bmp;
bmp=Bitmap::Create(size, size, 32, icon->Buffer);
m_Bitmaps.add(size, bmp);
return bmp;
}


//============================
// Con-/Destructors Protected
//============================

Icon::Icon(ICON const* resource):
m_Icons(resource)
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

}