//===============
// ScrollBox.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "UI/Frame.h"
#include "ScrollBox.h"

using namespace Core;
using namespace Graphics;
using namespace UI::Controls;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

ScrollBox::ScrollBox(Window* parent):
Control(parent),
AlignHorizontal(HorizontalAlignment::Left),
AlignVertical(VerticalAlignment::Top),
Enabled(this, true),
ZoomMax(1.f),
ZoomMin(1.f),
m_StartPoint(-1, -1),
m_Zoom(1.f)
{
Body=new Interactive(this);
Body->PointerDown.Add(this, &ScrollBox::OnBodyPointerDown);
Body->PointerMoved.Add(this, &ScrollBox::OnBodyPointerMoved);
Body->PointerUp.Add(this, &ScrollBox::OnBodyPointerUp);
Body->PointerWheel.Add(this, &ScrollBox::OnBodyPointerWheel);
VerticalBar=new ScrollBar(this, Orientation::Vertical);
VerticalBar->Scrolled.Add(this, &ScrollBox::OnScrollBarScrolled);
VerticalBar->Visible=false;
HorizontalBar=new ScrollBar(this, Orientation::Horizontal);
HorizontalBar->Scrolled.Add(this, &ScrollBox::OnScrollBarScrolled);
HorizontalBar->Visible=false;
}


//========
// Common
//========

Graphics::SIZE ScrollBox::GetMinSize(RenderTarget* target)
{
SIZE size(1, 1);
FLOAT scale=GetScaleFactor();
return size.Max(MinSize*scale);
}

Graphics::POINT ScrollBox::GetPosition()
{
auto content=Body->GetVisibleChild(0);
if(!content)
	return POINT(0, 0);
RECT content_rect=content->GetRect();
return POINT(-content_rect.Left, -content_rect.Top);
}

VOID ScrollBox::Rearrange(RenderTarget* target, RECT& rc)
{
rc.SetPadding(1, 1, 1, 1);
auto content=Body->GetVisibleChild(0);
if(!content)
	return;
RECT content_rect=content->GetRect();
SIZE min_size=content->GetMinSize(target);
content_rect.SetMinSize(min_size);
SIZE content_size=content_rect.GetSize();
RECT body_rect=rc;
BOOL scroll_h=false;
BOOL scroll_v=false;
UpdateBodyRect(body_rect, content_size, &scroll_h, &scroll_v);
UpdateBodyRect(body_rect, content_size, &scroll_h, &scroll_v);
Body->Move(target, body_rect);
SIZE body_size=body_rect.GetSize();
SetHotspot(content_rect, body_rect);
UpdateContentRect(content_rect, body_size);
content->Move(target, content_rect);
if(scroll_h)
	{
	RECT rc_bar(body_rect.Left, rc.Bottom-HorizontalBar->GetWidth(), body_rect.Right, rc.Bottom);
	HorizontalBar->Move(target, rc_bar);
	HorizontalBar->Fraction=(FLOAT)body_size.Width/content_size.Width;
	HorizontalBar->Position=-content_rect.Left;
	HorizontalBar->Range=content_size.Width-body_size.Width;
	}
if(scroll_v)
	{
	RECT rc_bar(rc.Right-VerticalBar->GetWidth(), body_rect.Top, rc.Right, body_rect.Bottom);
	VerticalBar->Move(target, rc_bar);
	VerticalBar->Fraction=(FLOAT)body_size.Height/content_size.Height;
	VerticalBar->Position=-content_rect.Top;
	VerticalBar->Range=content_size.Height-body_size.Height;
	}
HorizontalBar->Visible=scroll_h;
VerticalBar->Visible=scroll_v;
}

VOID ScrollBox::Render(RenderTarget* target, RECT& rc)
{
Control::Render(target, rc);
auto theme=GetTheme();
auto border=theme->GetBorderBrush();
target->DrawRect(rc, border);
rc.SetPadding(1, 1, 1, 1);
}

VOID ScrollBox::Scroll(INT x, INT y)
{
m_Hotspot.Clear();
POINT pos=GetPosition();
pos.Left+=x;
pos.Top+=y;
SetPosition(pos.Left, pos.Top);
}

VOID ScrollBox::SetHotspot(RECT rc_hot)
{
m_Hotspot=rc_hot;
Invalidate(true);
}

VOID ScrollBox::SetPosition(INT left, INT top)
{
auto content=Body->GetVisibleChild(0);
if(!content)
	return;
left=Max(left, 0);
top=Max(top, 0);
RECT body_rect=Body->GetClientRect();
SIZE body_size=body_rect.GetSize();
RECT content_rect=content->GetRect();
SIZE content_size=content_rect.GetSize();
if(content_size.Width>body_size.Width)
	{
	left=Min(left, content_size.Width-body_size.Width);
	}
else
	{
	left=0;
	}
if(content_size.Height>body_size.Height)
	{
	top=Min(top, content_size.Height-body_size.Height);
	}
else
	{
	top=0;
	}
if(content_rect.Left==-left&&content_rect.Top==-top)
	return;
content_rect.SetPosition(-left, -top);
content->Move(content_rect);
}

VOID ScrollBox::Zoom(FLOAT zoom)
{
zoom=Max(ZoomMin, zoom);
zoom=Min(ZoomMax, zoom);
m_Zoom=zoom;
Scale=m_Zoom;
Invalidate(true);
}


//================
// Common Private
//================

VOID ScrollBox::OnBodyPointerDown(Handle<PointerEventArgs> args)
{
if(args->Button!=PointerButton::Wheel)
	return;
auto frame=GetFrame();
BOOL ctrl=frame->IsKeyDown(VirtualKey::Control);
if(ctrl)
	{
	Zoom(1.f);
	args->Handled=true;
	return;
	}
m_StartPoint=args->Point;
m_StartPosition=GetPosition();
Body->CapturePointer();
args->Handled=true;
}

VOID ScrollBox::OnBodyPointerMoved(Handle<PointerEventArgs> args)
{
if(m_StartPoint.Left==-1)
	return;
POINT const& pt=args->Point;
INT delta_x=pt.Left-m_StartPoint.Left;
INT delta_y=pt.Top-m_StartPoint.Top;
INT left=m_StartPosition.Left-delta_x;
INT top=m_StartPosition.Top-delta_y;
SetPosition(left, top);
args->Handled=true;
}

VOID ScrollBox::OnBodyPointerUp(Handle<PointerEventArgs> args)
{
if(args->Button!=PointerButton::Wheel)
	return;
Body->ReleasePointer();
m_StartPoint.Set(-1, -1);
m_StartPosition.Set(0, 0);
args->Handled=true;
}

VOID ScrollBox::OnBodyPointerWheel(Handle<PointerEventArgs> args)
{
if(args->Handled)
	return;
INT delta=args->Delta;
auto frame=GetFrame();
BOOL ctrl=frame->IsKeyDown(VirtualKey::Control);
if(ctrl)
	{
	ZoomStep(delta);
	}
else
	{
	BOOL shift=frame->IsKeyDown(VirtualKey::Shift);
	INT x=0;
	INT y=0;
	if(VerticalBar->Visible)
		y=-delta;
	if(HorizontalBar->Visible)
		x=-delta;
	if(x!=0&&y!=0)
		{
		Scroll(shift? x: 0, shift? 0: y);
		}
	else if(x!=0||y!=0)
		{
		Scroll(x, y);
		}
	}
args->Handled=true;
}

VOID ScrollBox::OnScrollBarScrolled()
{
m_Hotspot.Clear();
SetPosition(HorizontalBar->Position, VerticalBar->Position);
}

VOID ScrollBox::SetHotspot(RECT& rc_content, RECT const& rc_body)
{
if(!m_Hotspot)
	return;
RECT rc_hotspot(m_Hotspot);
rc_hotspot.Move(rc_content.Left, rc_content.Top);
rc_hotspot.SetBounds(rc_content);
INT move_x=0;
INT move_y=0;
if(rc_hotspot.Left<0)
	{
	move_x=-rc_hotspot.Left;
	}
else if(rc_hotspot.Right>rc_body.Right)
	{
	move_x=-(rc_hotspot.Right-rc_body.Right);
	}
if(rc_hotspot.Top<0)
	{
	move_y=-rc_hotspot.Top;
	}
else if(rc_hotspot.Bottom>rc_body.Bottom)
	{
	move_y=-(rc_hotspot.Bottom-rc_body.Bottom);
	}
rc_content.Move(move_x, move_y);
}

VOID ScrollBox::UpdateBodyRect(RECT& rc_body, SIZE const& content_size, BOOL* scroll_h_ptr, BOOL* scroll_v_ptr)
{
BOOL scroll_h=*scroll_h_ptr;
BOOL scroll_v=*scroll_v_ptr;
SIZE body_size=rc_body.GetSize();
if(!scroll_h)
	{
	switch(HorizontalBar->Visibility)
		{
		case ScrollBarVisibility::Auto:
			{
			if(content_size.Width>body_size.Width)
				scroll_h=true;
			break;
			}
		case ScrollBarVisibility::Hidden:
			{
			break;
			}
		case ScrollBarVisibility::Visible:
			{
			scroll_h=true;
			break;
			}
		}
	if(scroll_h)
		{
		*scroll_h_ptr=true;
		rc_body.Bottom-=HorizontalBar->GetWidth();
		}
	}
if(!scroll_v)
	{
	switch(VerticalBar->Visibility)
		{
		case ScrollBarVisibility::Auto:
			{
			if(content_size.Height>body_size.Height)
				scroll_v=true;
			break;
			}
		case ScrollBarVisibility::Hidden:
			{
			break;
			}
		case ScrollBarVisibility::Visible:
			{
			scroll_v=true;
			break;
			}
		}
	if(scroll_v)
		{
		*scroll_v_ptr=true;
		rc_body.Right-=VerticalBar->GetWidth();
		}
	}
}

VOID ScrollBox::UpdateContentRect(RECT& rc_content, SIZE const& body_size)
{
SIZE content_size=rc_content.GetSize();
INT move_x=0;
INT move_y=0;
if(content_size.Width>body_size.Width)
	{
	if(rc_content.Left>0)
		move_x=-rc_content.Left;
	if(rc_content.Right<body_size.Width)
		move_x=body_size.Width-rc_content.Right;
	}
else
	{
	switch(AlignHorizontal)
		{
		case HorizontalAlignment::Left:
			{
			rc_content.Right=rc_content.Left+content_size.Width;
			break;
			}
		case HorizontalAlignment::Right:
			{
			rc_content.Left=rc_content.Right-content_size.Width;
			break;
			}
		case HorizontalAlignment::Center:
			{
			UINT space=body_size.Width-content_size.Width;
			rc_content.Left+=space/2;
			rc_content.Right=rc_content.Left+content_size.Width;
			break;
			}
		case HorizontalAlignment::Stretch:
			{
			rc_content.Right=rc_content.Left+body_size.Width;
			break;
			}
		}
	}
if(content_size.Height>body_size.Height)
	{
	if(rc_content.Top>0)
		move_y=-rc_content.Top;
	if(rc_content.Bottom<body_size.Height)
		move_y=body_size.Height-rc_content.Bottom;
	}
else
	{
	switch(AlignVertical)
		{
		case VerticalAlignment::Top:
			{
			rc_content.Bottom=rc_content.Top+content_size.Height;
			break;
			}
		case VerticalAlignment::Bottom:
			{
			rc_content.Top=rc_content.Bottom-content_size.Height;
			break;
			}
		case VerticalAlignment::Center:
			{
			UINT space=body_size.Height-content_size.Height;
			rc_content.Top+=space/2;
			rc_content.Bottom=rc_content.Top+content_size.Height;
			break;
			}
		case VerticalAlignment::Stretch:
			{
			rc_content.Bottom=rc_content.Top+body_size.Height;
			break;
			}
		}
	}
rc_content.Move(move_x, move_y);
}

VOID ScrollBox::ZoomStep(INT step)
{
FLOAT zoom=m_Zoom;
if(step>0)
	{
	zoom*=1.1f;
	}
else
	{
	zoom/=1.1f;
	}
Zoom(zoom);
}

}}