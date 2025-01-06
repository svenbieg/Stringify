//===============
// SplitView.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "SplitView.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Handle<Cursor> SplitView::GetCursor()
{
auto theme=GetTheme();
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		return theme->SizeVerticalCursor;
	case Orientation::Vertical:
		return theme->SizeHorizontalCursor;
	}
return nullptr;
}

VOID SplitView::Rearrange(RenderTarget* target, RECT& rc)
{
auto child0=GetVisibleChild(0);
auto child1=GetVisibleChild(1);
if(!child0)
	{
	if(!child1)
		return;
	child1->Move(target, rc);
	return;
	}
if(!child1)
	{
	child0->Move(target, rc);
	return;
	}
RECT rc1(rc);
RECT rc2(rc);
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		{
		rc1.Bottom=rc1.Top+Size;
		rc2.Top=rc1.Bottom+Distance;
		break;
		}
	case Orientation::Vertical:
		{
		rc1.Right=rc1.Left+Size;
		rc2.Left=rc1.Right+Distance;
		break;
		}
	}
child0->Move(target, rc1);
child1->Move(target, rc2);
}


//==========================
// Con-/Destructors Private
//==========================

SplitView::SplitView(Window* parent, Orientation orientation):
Interactive(parent),
Distance(0),
Size(0),
m_StartSize(0),
m_Orientation(orientation)
{
FLOAT scale=GetScaleFactor();
Distance=8*scale;
Size=200*scale;
PointerDown.Add(this, &SplitView::OnPointerDown);
PointerMoved.Add(this, &SplitView::OnPointerMoved);
PointerUp.Add(this, &SplitView::OnPointerUp);
}


//================
// Common Private
//================

VOID SplitView::OnPointerDown(Handle<PointerEventArgs> args)
{
if(args->Button!=PointerButton::Left)
	return;
m_StartSize=Size;
m_StartPoint=args->Point;
CapturePointer();
args->Handled=true;
}

VOID SplitView::OnPointerMoved(Handle<PointerEventArgs> args)
{
if(m_StartSize==0)
	return;
POINT const& pt=args->Point;
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		{
		UINT height=m_Rect.GetHeight();
		Size+=m_StartPoint.Top-pt.Top;
		Size=TypeHelper::Max(Size, 20U);
		Size=TypeHelper::Min(Size, height-20);
		break;
		}
	case Orientation::Vertical:
		{
		UINT width=m_Rect.GetWidth();
		Size+=m_StartPoint.Left-pt.Left;
		Size=TypeHelper::Max(Size, 20U);
		Size=TypeHelper::Min(Size, width-20);
		break;
		}
	}
Invalidate(true);
}

VOID SplitView::OnPointerUp(Handle<PointerEventArgs> args)
{
if(args->Button!=PointerButton::Left)
	return;
m_StartSize=0;
ReleasePointer();
args->Handled=true;
}

}}
