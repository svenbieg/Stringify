//===============
// HyperLink.cpp
//===============

#include "HyperLink.h"


//=======
// Using
//=======

#include "Environment.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Handle<Cursor> HyperLink::GetCursor()
{
if(!m_Theme)
	return nullptr;
return m_Theme->HandPointCursor;
}

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
auto brush=Color;
if(HasFocus())
	brush=m_Theme->HighlightBrush;
target->DrawText(rc, scale, font, brush, Text->Begin());
target->DrawLine(POINT(rc.Left, rc.Bottom), POINT(rc.Right, rc.Bottom), Color);
}


//==========================
// Con-/Destructors Private
//==========================

HyperLink::HyperLink(Window* parent):
Interactive(parent)
{
Clicked.Add(this, &HyperLink::OnClicked);
Color=Brush::Create(Colors::Blue);
TabStop=true;
}


//================
// Common Private
//================

VOID HyperLink::OnClicked()
{
auto link=Link;
if(!link)
	link=Text;
Environment::Open(link);
}

}}