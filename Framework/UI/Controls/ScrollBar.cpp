//===============
// ScrollBar.cpp
//===============

#include "UI/Controls/ScrollBar.h"


//=======
// Using
//=======

#include "Timing/Clock.h"

using namespace Devices::Timers;
using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

ScrollBar::~ScrollBar()
{
StopScrolling();
}


//========
// Common
//========

Graphics::SIZE ScrollBar::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
UINT width=Width*scale;
SIZE size(0, 0);
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		{
		size.Height=width;
		break;
		}
	case Orientation::Vertical:
		{
		size.Width=width;
		break;
		}
	}
return size.Max(MinSize*scale);
}

UINT ScrollBar::GetWidth()
{
FLOAT scale=GetScaleFactor();
return Width*scale;
}

VOID ScrollBar::Render(RenderTarget* target, RECT& rc)
{
auto br_control=m_Theme->ControlBrush;
auto br_highlight=m_Theme->HighlightBrush;
FLOAT scale=GetScaleFactor();
UINT width=Width*scale;
UINT padding=width*0.2f;
UINT btn_size=width-3*padding;
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		{
		auto color=(m_Highlight==ScrollBarButton::First)? br_highlight: br_control;
		POINT pts[3];
		pts[0].Set(rc.Left+padding, rc.Top+width/2);
		pts[1].Set(rc.Left+padding+btn_size, rc.Top+padding);
		pts[2].Set(rc.Left+padding+btn_size, rc.Bottom-padding);
		target->FillPolygon(pts, 3, color);
		color=(m_Highlight==ScrollBarButton::Second)? br_highlight: br_control;
		pts[0].Set(rc.Right-padding, rc.Top+width/2);
		pts[1].Set(rc.Right-padding-btn_size, rc.Top+padding);
		pts[2].Set(rc.Right-padding-btn_size, rc.Bottom-padding);
		target->FillPolygon(pts, 3, color);
		if(Fraction>0.f)
			{
			color=(m_Highlight==ScrollBarButton::Bar)? br_highlight: br_control;
			UINT size=rc.Right-2*width;
			UINT bar=size*Fraction;
			bar=TypeHelper::Max(bar, width);
			UINT scroll=size-bar;
			RECT rc_bar(rc);
			rc_bar.Top+=padding;
			rc_bar.Bottom-=padding;
			rc_bar.Left+=width;
			rc_bar.Left+=(FLOAT)Position*scroll/Range;
			rc_bar.Right=rc_bar.Left+bar;
			target->FillRect(rc_bar, color);
			}
		break;
		}
	case Orientation::Vertical:
		{
		auto color=(m_Highlight==ScrollBarButton::First)? br_highlight: br_control;
		POINT pts[3];
		pts[0].Set(rc.Left+width/2, rc.Top+padding);
		pts[1].Set(rc.Left+padding, rc.Top+padding+btn_size);
		pts[2].Set(rc.Right-padding, rc.Top+padding+btn_size);
		target->FillPolygon(pts, 3, color);
		color=(m_Highlight==ScrollBarButton::Second)? br_highlight: br_control;
		pts[0].Set(rc.Left+width/2, rc.Bottom-padding);
		pts[1].Set(rc.Left+padding, rc.Bottom-padding-btn_size);
		pts[2].Set(rc.Right-padding, rc.Bottom-padding-btn_size);
		target->FillPolygon(pts, 3, color);
		if(Fraction>0.f)
			{
			color=(m_Highlight==ScrollBarButton::Bar)? br_highlight: br_control;
			UINT size=rc.Bottom-2*width;
			UINT bar=size*Fraction;
			bar=TypeHelper::Max(bar, width);
			UINT scroll=size-bar;
			RECT rc_bar(rc);
			rc_bar.Left+=padding;
			rc_bar.Right-=padding;
			rc_bar.Top+=width;
			rc_bar.Top+=(FLOAT)Position*scroll/Range;
			rc_bar.Bottom=rc_bar.Top+bar;
			target->FillRect(rc_bar, color);
			}
		break;
		}
	}
}


//==========================
// Con-/Destructors Private
//==========================

ScrollBar::ScrollBar(Window* parent, Orientation orientation):
Interactive(parent),
Fraction(0.f),
Position(0),
Range(0),
Step(10),
Visibility(ScrollBarVisibility::Auto),
Width(12),
m_Highlight(ScrollBarButton::None),
m_Orientation(orientation),
m_Start(0),
m_StartPoint(-1, -1),
m_Step(0)
{
PointerDown.Add(this, &ScrollBar::OnPointerDown);
PointerLeft.Add(this, &ScrollBar::OnPointerLeft);
PointerMoved.Add(this, &ScrollBar::OnPointerMoved);
PointerUp.Add(this, &ScrollBar::OnPointerUp);
}


//================
// Common Private
//================

ScrollBarButton ScrollBar::GetButton(POINT const& pt)
{
FLOAT scale=GetScaleFactor();
UINT width=Width*scale;
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		{
		if(pt.Left<width)
			return ScrollBarButton::First;
		if(pt.Left>m_Rect.GetWidth()-width)
			return ScrollBarButton::Second;
		return ScrollBarButton::Bar;
		}
	case Orientation::Vertical:
		{
		if(pt.Top<width)
			return ScrollBarButton::First;
		if(pt.Top>m_Rect.GetHeight()-width)
			return ScrollBarButton::Second;
		return ScrollBarButton::Bar;
		}
	}
return ScrollBarButton::None;
}

VOID ScrollBar::OnPointerDown(Handle<PointerEventArgs> args)
{
POINT const& pt=args->Point;
ScrollBarButton button=GetButton(pt);
switch(button)
	{
	case ScrollBarButton::First:
		{
		StartScrolling(-Step);
		args->Handled=true;
		break;
		}
	case ScrollBarButton::Second:
		{
		StartScrolling(Step);
		args->Handled=true;
		break;
		}
	case ScrollBarButton::Bar:
		{
		m_Start=Position;
		m_StartPoint=pt;
		CapturePointer();
		args->Handled=true;
		break;
		}
	default:
		{
		break;
		}
	}
}

VOID ScrollBar::OnPointerLeft()
{
if(m_Highlight==ScrollBarButton::None)
	return;
m_Highlight=ScrollBarButton::None;
Invalidate();
}

VOID ScrollBar::OnPointerMoved(Handle<PointerEventArgs> args)
{
POINT const& pt=args->Point;
if(m_StartPoint.Left==-1)
	{
	auto highlight=GetButton(args->Point);
	if(m_Highlight!=highlight)
		{
		m_Highlight=highlight;
		Invalidate();
		}
	args->Handled=true;
	return;
	}
INT start=0;
INT pos=0;
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		{
		start=m_StartPoint.Left;
		pos=pt.Left;
		break;
		}
	case Orientation::Vertical:
		{
		start=m_StartPoint.Top;
		pos=pt.Top;
		break;
		}
	}
INT move=pos-start;
move/=Fraction;
pos=(INT)m_Start+move;
if(pos<0)
	pos=TypeHelper::Max(pos, 0);
pos=TypeHelper::Min(pos, (INT)Range);
Position=pos;
Scrolled(this);
args->Handled=true;
}

VOID ScrollBar::OnPointerUp(Handle<PointerEventArgs> args)
{
StopScrolling();
if(m_StartPoint.Left!=-1)
	{
	ReleasePointer();
	m_StartPoint.Set(-1, -1);
	args->Handled=true;
	}
}

VOID ScrollBar::OnSystemTimer()
{
INT pos=Position+m_Step;
pos=TypeHelper::Max(pos, 0);
pos=TypeHelper::Min(pos, (INT)Range);
if(Position==pos)
	{
	StopScrolling();
	return;
	}
Position=pos;
Scrolled(this);
}

VOID ScrollBar::StartScrolling(INT step)
{
if(!m_Timer)
	{
	m_Timer=SystemTimer::Get();
	m_Timer->Triggered.Add(this, &ScrollBar::OnSystemTimer);
	}
m_Step=step;
}

VOID ScrollBar::StopScrolling()
{
if(m_Timer)
	{
	m_Timer->Triggered.Remove(this);
	m_Timer=nullptr;
	}
m_Step=0;
}

}}