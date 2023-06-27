//==========
// Icon.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Buffer.h"
#include "Icon.h"
#include "ImagingFactory.h"

using namespace Clusters;
using namespace Resources::Icons;
using namespace Storage;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//==================
// Con-/Destructors
//==================

Icon::Icon(WORD id):
Graphics::Icon(nullptr),
uId(id)
{
UINT count=GetResourceIconCount(id);
UINT* sizes=new UINT[count];
GetResourceIconSize(id, sizes, count);
ICON* icons=new ICON[count+1];
for(UINT u=0; u<count; u++)
	{
	UINT size=sizes[u];
	HINSTANCE instance=GetModuleHandle(nullptr);
	HICON hicon=(HICON)LoadImage(instance, MAKEINTRESOURCE(uId), IMAGE_ICON, size, size, 0);
	ICONINFO info;
	GetIconInfo(hicon, &info);
	UINT buf_size=GetBitmapBits(info.hbmColor, 0, nullptr);
	LPSTR buf=new CHAR[buf_size];
	GetBitmapBits(info.hbmColor, buf_size, buf);
	icons[u].Size=size;
	icons[u].Buffer=buf;
	cIcons.add(size, hicon);
	}
icons[count].Size=0;
icons[count].Buffer=nullptr;
pIcons=icons;
}

Icon::Icon(ICON const* icons):
Graphics::Icon(icons),
uId(0)
{
auto icon=pIcons;
while(icon->Size)
	{
	UINT size=icon->Size;
	HBITMAP bitmap=CreateBitmap(size, size, 1, 32, icon->Buffer);
	Handle<Buffer> buf=new Buffer(size*size*4);
	buf->Fill(0xFFFFFFFF);
	HBITMAP mask=CreateBitmap(size, size, 1, 32, buf->Begin());
	ICONINFO info;
	ZeroMemory(&info, sizeof(ICONINFO));
	info.fIcon=TRUE;
	info.hbmColor=bitmap;
	info.hbmMask=mask;
	HICON hicon=CreateIconIndirect(&info);
	cIcons.add(size, hicon);
	icon++;
	}
}

Icon::~Icon()
{
if(uId)
	{
	auto icon=pIcons;
	while(icon->Size)
		{
		delete icon->Buffer;
		icon++;
		}
	delete pIcons;
	}
for(auto it=cIcons.cbegin(); it.has_current(); it.move_next())
	{
	HICON icon=it->get_value();
	DestroyIcon(icon);
	}
}


//========
// Common
//========

HICON Icon::GetHandle(UINT size)
{
auto it=cIcons.find(size, find_func::below_or_equal);
if(!it.has_current())
	return NULL;
return it->get_value();
}

}}