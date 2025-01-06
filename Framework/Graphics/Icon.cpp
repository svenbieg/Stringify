//==========
// Icon.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Icon.h"

using namespace Resources::Icons;


//===========
// Namespace
//===========

namespace Graphics {


//========
// Common
//========

Handle<Graphics::Bitmap> Icon::GetBitmap(UINT size)
{
auto icon=GetIcon(size);
if(!icon)
	return nullptr;
size=icon->Size;
return Bitmap::Create(size, size, size*size*4, icon->Buffer);
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