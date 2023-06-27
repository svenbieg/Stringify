//============
// Window.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Frame.h"

using namespace Graphics;
using namespace UI::Controls;


//===========
// Namespace
//===========

namespace UI {


//========
// Common
//========

VOID Window::BringToFront()
{
if(!Parent)
	return;
auto children=Parent->Children;
children->Remove(this, false);
children->Append(this, false);
Invalidate(true);
}

Handle<Brush> Window::GetBackgroundBrush()
{
return Background;
}

Graphics::RECT Window::GetClientRect()const
{
return RECT(0, 0, rcRect.GetWidth(), rcRect.GetHeight());
}

Handle<Graphics::Font> Window::GetFont()
{
if(Font)
	return Font;
if(Parent)
	return Parent->GetFont();
auto theme=GetTheme();
return theme->GetDefaultFont();
}

Frame* Window::GetFrame()
{
if(!Parent)
	return nullptr;
return Parent->GetFrame();
}

Graphics::POINT Window::GetFrameOffset()const
{
POINT pt(rcRect.Left, rcRect.Top);
if(Parent)
	pt+=Parent->GetFrameOffset();
return pt;
}

Graphics::RECT Window::GetFrameRect()const
{
POINT offset=this->GetFrameOffset();
SIZE size=rcRect.GetSize();
RECT rc_frame;
rc_frame.Left=offset.Left;
rc_frame.Top=offset.Top;
rc_frame.Right=offset.Left+size.Width;
rc_frame.Bottom=offset.Top+size.Height;
return rc_frame;
}

Graphics::SIZE Window::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
SIZE size(0, 0);
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	SIZE child_size=child->GetMinSize(target);
	auto control=Convert<Control>(child);
	if(control)
		child_size.AddPadding(control->Margin*scale);
	size.Width=MAX(size.Width, child_size.Width);
	size.Height=MAX(size.Height, child_size.Height);
	}
return size.Max(MinSize*scale);
}

Graphics::POINT Window::GetOffset()const
{
return POINT(rcRect.Left, rcRect.Top);
}

FLOAT Window::GetScaleFactor()const
{
if(!Parent)
	return Scale;
FLOAT scale=Parent->GetScaleFactor();
return Scale*scale;
}

Handle<Graphics::RenderTarget> Window::GetTarget()
{
auto frame=this->GetFrame();
if(!frame)
	return nullptr;
return frame->GetTarget();
}

Handle<Graphics::Theme> Window::GetTheme()
{
auto frame=this->GetFrame();
if(!frame)
	return Theme::Default;
return frame->GetTheme();
}

Handle<Window> Window::GetVisibleChild(UINT id)
{
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	if(id==0)
		return child;
	id--;
	}
return nullptr;
}

VOID Window::Invalidate(BOOL rearrange)
{
SetFlag(uFlags, WindowFlags::Repaint);
if(GetFlag(uFlags, WindowFlags::Rearrange))
	return;
if(rearrange)
	SetFlag(uFlags, WindowFlags::Rearrange);
if(Parent)
	{
	BOOL transparent=true;
	auto background=this->GetBackgroundBrush();
	if(background)
		{
		auto c=background->GetColor();
		if(!c.IsTransparent())
			transparent=false;
		}
	transparent|=rearrange;
	if(transparent)
		{
		Parent->Invalidate(rearrange);
		return;
		}
	}
auto frame=this->GetFrame();
frame->Invalidate(rearrange);
}

BOOL Window::IsParentOf(Window* window)
{
if(!window)
	return false;
Window* parent=window->Parent;
while(parent)
	{
	if(parent==this)
		return true;
	parent=parent->Parent;
	}
return false;
}

BOOL Window::IsVisible()
{
if(!Visible)
	return false;
if(!Parent)
	return true;
return Parent->IsVisible();
}

VOID Window::Move(RECT const& rc)
{
if(rcRect!=rc)
	{
	rcRect=rc;
	Invalidate(true);
	}
}

VOID Window::Move(RenderTarget* target, RECT const& rc)
{
if(rcRect!=rc)
	{
	rcRect=rc;
	Invalidate(true);
	}
if(GetFlag(uFlags, WindowFlags::Rearrange))
	{
	ClearFlag(uFlags, WindowFlags::Rearrange);
	RECT rc_client=GetClientRect();
	this->Rearrange(target, rc_client);
	return;
	}
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	RECT rc=child->GetRect();
	child->Move(target, rc);
	}
}

VOID Window::Render(RenderTarget* target, RECT& rc)
{
auto background=this->GetBackgroundBrush();
if(background)
	{
	RECT rc_fill(rc);
	auto offset=target->GetOffset();
	rc_fill.Move(offset);
	target->FillRect(rc_fill, background);
	}
}

VOID Window::SetPosition(POINT const& pt)
{
UINT width=rcRect.GetWidth();
UINT height=rcRect.GetHeight();
rcRect.Set(pt.Left, pt.Top, pt.Left+width, pt.Top+height);
Invalidate(true);
}


//============================
// Con-/Destructors Protected
//============================

Window::Window(Window* parent):
MinSize(0, 0),
Parent(this, parent),
Scale(1.f),
Visible(this, true),
// Protected
rcRect(0, 0, 0, 0),
// Private
pOldParent(parent),
uFlags(WindowFlags::None)
{
Children=new ChildList();
Parent.Changed.Add(this, &Window::OnParentChanged);
Visible.Changed.Add(this, &Window::OnVisibleChanged);
if(Parent)
	{
	Parent->Children->Append(this);
	Font=Parent->Font;
	}
}


//================
// Common Private
//================

VOID Window::OnParentChanged(Window* parent)
{
if(pOldParent)
	{
	pOldParent->Children->Remove(this);
	pOldParent->Invalidate(true);
	}
pOldParent=parent;
if(!parent)
	return;
parent->Children->Append(this);
Invalidate(true);
}

VOID Window::OnVisibleChanged(BOOL visible)
{
ClearFlag(uFlags, WindowFlags::Rearrange);
if(visible)
	{
	Invalidate(true);
	}
else
	{
	auto frame=GetFrame();
	auto focus=frame->GetFocus();
	if(focus)
		{
		if(!focus->IsVisible())
			frame->SetFocus(nullptr);
		}
	if(Parent)
		Parent->Invalidate(true);
	}
}

}