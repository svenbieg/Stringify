//===============
// SplitView.cpp
//===============

#include "SplitView.h"


//=======
// Using
//=======

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
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		return m_Theme->SizeHorizontalCursor;
	case Orientation::Vertical:
		return m_Theme->SizeVerticalCursor;
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
		rc1.Right=rc1.Left+Size;
		rc2.Left=rc1.Right+Distance;
		break;
		}
	case Orientation::Vertical:
		{
		rc1.Bottom=rc1.Top+Size;
		rc2.Top=rc1.Bottom+Distance;
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
m_Delta(0),
m_Orientation(orientation)
{
FLOAT scale=GetScaleFactor();
Distance=4*scale;
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
m_StartPoint=args->Point;
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		{
		m_Delta=m_StartPoint.Left-Size;
		break;
		}
	case Orientation::Vertical:
		{
		m_Delta=m_StartPoint.Top-Size;
		break;
		}
	}
CapturePointer();
args->Handled=true;
}

VOID SplitView::OnPointerMoved(Handle<PointerEventArgs> args)
{
if(!IsCapturingPointer())
	return;
POINT const& pt=args->Point;
switch(m_Orientation)
	{
	case Orientation::Horizontal:
		{
		UINT width=m_Rect.GetWidth();
		UINT left=TypeHelper::Max(pt.Left, 20);
		left=TypeHelper::Min(left, width-20);
		Size=left-m_Delta;
		break;
		}
	case Orientation::Vertical:
		{
		UINT height=m_Rect.GetHeight();
		UINT top=TypeHelper::Max(pt.Top, 20);
		top=TypeHelper::Min(top, height-20);
		Size=top-m_Delta;
		break;
		}
	}
Invalidate(true);
}

VOID SplitView::OnPointerUp(Handle<PointerEventArgs> args)
{
if(args->Button!=PointerButton::Left)
	return;
ReleasePointer();
args->Handled=true;
}

}}
