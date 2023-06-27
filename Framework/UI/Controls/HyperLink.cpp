//===============
// HyperLink.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "HyperLink.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

HyperLink::HyperLink(Window* parent):
Interactive(parent)
{}


//========
// Common
//========

Graphics::SIZE HyperLink::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
auto font=GetFont();
FLOAT scale=GetScaleFactor();
if(Text)
	{
	size=target->MeasureText(font, scale, Text->Begin());
	}
else
	{
	size.Height=font->GetSize()*scale;
	}
return size.Max(MinSize*scale);
}

VOID HyperLink::Render(RenderTarget* target, RECT& rc)
{
Interactive::Render(target, rc);
if(!Text)
	return;
auto theme=GetTheme();
auto color=theme->GetBrush(Colors::Blue);
FLOAT scale=GetScaleFactor();
target->TextColor=color;
target->Font=GetFont();
target->DrawText(rc, scale, Text->Begin());
target->DrawLine(POINT(rc.Left, rc.Bottom), POINT(rc.Right, rc.Bottom), color);
}

}}