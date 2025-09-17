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


//========
// Common
//========

Graphics::SIZE HyperLink::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
FLOAT scale=GetScaleFactor();
auto font=m_Theme->DefaultFont;
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
FLOAT scale=GetScaleFactor();
auto font=m_Theme->DefaultFont;
target->DrawText(rc, scale, font, Color, Text->Begin());
target->DrawLine(POINT(rc.Left, rc.Bottom), POINT(rc.Right, rc.Bottom), Color);
}


//==========================
// Con-/Destructors Private
//==========================

HyperLink::HyperLink(Window* parent):
Interactive(parent)
{
Color=Brush::Create(Colors::Blue);
}

}}