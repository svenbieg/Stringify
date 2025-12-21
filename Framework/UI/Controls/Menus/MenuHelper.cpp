//================
// MenuHelper.cpp
//================

#include "UI/Controls/Menus/MenuHelper.h"


//=======
// Using
//=======

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//========
// Common
//========

CHAR MenuHelper::GetAccelerator(LPCTSTR text)
{
if(!text)
	return 0;
for(UINT pos=0; text[pos]; pos++)
	{
	if(CharHelper::Compare(text[pos], '&')==0)
		return CharHelper::ToCapitalAnsi(text[pos+1]);
	}
return 0;
}

VOID MenuHelper::GetBitmapDisabled(Handle<Graphics::Bitmap> bmp)
{
auto size=bmp->GetDimensions();
for(UINT y=0; y<size.Height; y++)
	{
	for(UINT x=0; x<size.Width; x++)
		{
		COLOR c=bmp->GetPixel(x, y);
		if(c.GetAlpha()==0)
			continue;
		auto pc=(BYTE*)&c;
		BYTE b=(UINT)(pc[0]+pc[1]+pc[2])/3;
		if(b>=0x80)
			{
			b=(BYTE)(128.f+(b-128.f)*0.5f);
			}
		else
			{
			b=(BYTE)(128.f-(128.f-b)*0.5f);
			}
		b=TypeHelper::Min(b+0x30, 0xFF);
		pc[0]=pc[1]=pc[2]=b;
		bmp->SetPixel(x, y, c);
		}
	}
}

Handle<String> MenuHelper::GetLabel(LPCTSTR text)
{
if(!text)
	return nullptr;
UINT len=0;
StringHelper::FindChar(text, '|', &len);
auto label=String::Create(len, text);
return label->Replace("&", "");
}

Handle<String> MenuHelper::GetShortcut(LPCTSTR text)
{
if(!text)
	return nullptr;
UINT pos=0;
if(StringHelper::FindChar(text, '|', &pos))
	return String::Create(&text[pos+1]);
return nullptr;
}

}}}