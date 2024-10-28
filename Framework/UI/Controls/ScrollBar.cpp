//===============
// ScrollBar.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "ScrollBar.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

ScrollBar::ScrollBar(Window* parent, Orientation orientation):
Interactive(parent),
Fraction(0.f),
Position(0),
Range(0),
Step(10),
Visibility(ScrollBarVisibility::Auto),
Width(12),
iStep(0),
m_StartPoint(-1, -1),
uHighlight(ScrollBarButton::None),
uOrientation(orientation),
uStart(0)
{
PointerDown.Add(this, &ScrollBar::OnPointerDown);
PointerLeft.Add(this, &ScrollBar::OnPointerLeft);
PointerMoved.Add(this, &ScrollBar::OnPointerMoved);
PointerUp.Add(this, &ScrollBar::OnPointerUp);
}


//========
// Common
//========

Handle<Brush> ScrollBar::GetBackgroundBrush()
{
auto theme=GetTheme();
return theme->LightControlBrush;
}

Graphics::SIZE ScrollBar::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
UINT width=Width*scale;
SIZE size(0, 0);
switch(uOrientation)
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
Interactive::Render(target, rc);
auto theme=GetTheme();
auto br_control=theme->ControlBrush;
auto br_highlight=theme->BorderBrush;
FLOAT scale=GetScaleFactor();
UINT width=Width*scale;
UINT padding=width*0.2f;
UINT btn_size=width-3*padding;
switch(uOrientation)
	{
	case Orientation::Horizontal:
		{
		auto color=(uHighlight==ScrollBarButton::First)? br_highlight: br_control;
		POINT pts[3];
		pts[0].Set(rc.Left+padding, rc.Top+width/2);
		pts[1].Set(rc.Left+padding+btn_size, rc.Top+padding);
		pts[2].Set(rc.Left+padding+btn_size, rc.Bottom-padding);
		target->FillPolygon(pts, 3, color);
		color=(uHighlight==ScrollBarButton::Second)? br_highlight: br_control;
		pts[0].Set(rc.Right-padding, rc.Top+width/2);
		pts[1].Set(rc.Right-padding-btn_size, rc.Top+padding);
		pts[2].Set(rc.Right-padding-btn_size, rc.Bottom-padding);
		target->FillPolygon(pts, 3, color);
		if(Fraction>0.f)
			{
			color=(uHighlight==ScrollBarButton::Bar)? br_highlight: br_control;
			UINT size=rc.Right-2*width;
			UINT bar=size*Fraction;
			bar=Max(bar, width);
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
		auto color=(uHighlight==ScrollBarButton::First)? br_highlight: br_control;
		POINT pts[3];
		pts[0].Set(rc.Left+width/2, rc.Top+padding);
		pts[1].Set(rc.Left+padding, rc.Top+padding+btn_size);
		pts[2].Set(rc.Right-padding, rc.Top+padding+btn_size);
		target->FillPolygon(pts, 3, color);
		color=(uHighlight==ScrollBarButton::Second)? br_highlight: br_control;
		pts[0].Set(rc.Left+width/2, rc.Bottom-padding);
		pts[1].Set(rc.Left+padding, rc.Bottom-padding-btn_size);
		pts[2].Set(rc.Right-padding, rc.Bottom-padding-btn_size);
		target->FillPolygon(pts, 3, color);
		if(Fraction>0.f)
			{
			color=(uHighlight==ScrollBarButton::Bar)? br_highlight: br_control;
			UINT size=rc.Bottom-2*width;
			UINT bar=size*Fraction;
			bar=Max(bar, width);
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


//================
// Common Private
//================

ScrollBarButton ScrollBar::GetButton(POINT const& pt)
{
FLOAT scale=GetScaleFactor();
UINT width=Width*scale;
switch(uOrientation)
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
		uStart=Position;
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
if(uHighlight==ScrollBarButton::None)
	return;
uHighlight=ScrollBarButton::None;
Invalidate();
}

VOID ScrollBar::OnPointerMoved(Handle<PointerEventArgs> args)
{
POINT const& pt=args->Point;
if(m_StartPoint.Left==-1)
	{
	auto highlight=GetButton(args->Point);
	if(uHighlight!=highlight)
		{
		uHighlight=highlight;
		Invalidate();
		}
	args->Handled=true;
	return;
	}
INT start=0;
INT pos=0;
switch(uOrientation)
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
pos=(INT)uStart+move;
if(pos<0)
	pos=Max(pos, 0);
pos=Min(pos, (INT)Range);
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

VOID ScrollBar::OnScrollTimer()
{
INT pos=Position+iStep;
pos=Max(pos, 0);
pos=Min(pos, (INT)Range);
if(Position==pos)
	{
	StopScrolling();
	return;
	}
Position=pos;
Scrolled(this);
if(!hScrollTimer)
	{
	hScrollTimer=new Timer();
	hScrollTimer->Triggered.Add(this, &ScrollBar::OnScrollTimer);
	hScrollTimer->StartPeriodic(100);
	}
}

VOID ScrollBar::StartScrolling(INT step)
{
iStep=step;
OnScrollTimer();
}

VOID ScrollBar::StopScrolling()
{
hScrollTimer=nullptr;
iStep=0;
}

}}
