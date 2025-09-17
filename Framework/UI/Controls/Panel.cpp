//===========
// Panel.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Panel.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Handle<Brush> Panel::GetBackground()
{
return Background;
}

Handle<Brush> Panel::GetBorderBrush()
{
if(!Border)
	return nullptr;
return m_Theme->BorderBrush;
}

Graphics::SIZE Panel::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
FLOAT scale=GetScaleFactor();
auto content=GetVisibleChild(0);
if(content)
	{
	size=content->GetMinSize(target);
	auto control=content.As<Control>();
	if(control)
		{
		RECT const& margin=control->Margin;
		size.AddPadding(margin*scale);
		}
	}
auto border=this->GetBorderBrush();
if(border)
	{
	size.Width+=2;
	size.Height+=2;
	}
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID Panel::Rearrange(RenderTarget* target, RECT& rc)
{
auto border=this->GetBorderBrush();
if(border)
	rc.SetPadding(1, 1, 1, 1);
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
auto content=GetVisibleChild(0);
if(!content)
	return;
RECT rc_content=rc;
auto control=content.As<Control>();
if(control)
	rc_content.SetPadding(control->Margin*scale);
content->Move(target, rc_content);
}

VOID Panel::Render(RenderTarget* target, RECT& rc)
{
Control::Render(target, rc);
auto border=this->GetBorderBrush();
if(border)
	{
	target->DrawRect(rc, border);
	rc.SetPadding(1, 1, 1, 1);
	}
}


//============================
// Con-/Destructors Protected
//============================

Panel::Panel(Window* parent):
Control(parent),
Border(false)
{}

}}