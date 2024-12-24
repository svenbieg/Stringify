//============
// Window.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "UI/Frame.h"

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
return RECT(0, 0, m_Rect.GetWidth(), m_Rect.GetHeight());
}

Handle<Graphics::Font> Window::GetFont()
{
if(Font)
	return Font;
if(Parent)
	return Parent->GetFont();
auto theme=GetTheme();
return theme->DefaultFont;
}

Frame* Window::GetFrame()
{
if(!Parent)
	return nullptr;
return Parent->GetFrame();
}

Graphics::POINT Window::GetFrameOffset()const
{
POINT pt(m_Rect.Left, m_Rect.Top);
if(Parent)
	pt+=Parent->GetFrameOffset();
return pt;
}

Graphics::RECT Window::GetFrameRect()const
{
POINT offset=this->GetFrameOffset();
return m_Rect.SetPosition(offset);
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
	size.Width=TypeHelper::Max(size.Width, child_size.Width);
	size.Height=TypeHelper::Max(size.Height, child_size.Height);
	}
return size.Max(MinSize*scale);
}

FLOAT Window::GetScaleFactor()const
{
if(!Parent)
	return Scale;
FLOAT scale=Parent->GetScaleFactor();
return Scale*scale;
}

Graphics::POINT Window::GetScreenOffset()const
{
POINT pt(m_Rect.Left, m_Rect.Top);
if(Parent)
	pt+=Parent->GetScreenOffset();
return pt;
}

Graphics::RECT Window::GetScreenRect()const
{
POINT offset=GetScreenOffset();
return m_Rect.SetPosition(offset);
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
auto frame=GetFrame();
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
FlagHelper::Set(m_Flags, WindowFlags::Repaint);
if(FlagHelper::Get(m_Flags, WindowFlags::Rearrange))
	return;
if(rearrange)
	FlagHelper::Set(m_Flags, WindowFlags::Rearrange);
BOOL transparent=false;
if(Parent)
	{
	transparent=true;
	auto background=this->GetBackgroundBrush();
	if(background)
		{
		auto c=background->GetColor();
		if(!c.IsTransparent())
			transparent=false;
		}
	transparent|=rearrange;
	}
if(transparent)
	{
	Parent->Invalidate(rearrange);
	}
else
	{
	auto frame=this->GetFrame();
	frame->Invalidate(rearrange);
	}
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
if(m_Rect!=rc)
	{
	m_Rect=rc;
	Invalidate(true);
	}
}

VOID Window::Move(RenderTarget* target, RECT const& rc)
{
if(m_Rect!=rc)
	{
	FlagHelper::Set(m_Flags, WindowFlags::Update);
	m_Rect=rc;
	}
if(FlagHelper::Get(m_Flags, WindowFlags::Rearrange))
	{
	FlagHelper::Clear(m_Flags, WindowFlags::Rearrange);
	RECT rc_client=GetClientRect();
	this->Rearrange(target, rc_client);
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
UINT width=m_Rect.GetWidth();
UINT height=m_Rect.GetHeight();
m_Rect.Set(pt.Left, pt.Top, pt.Left+width, pt.Top+height);
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
m_Flags(WindowFlags::None),
m_Rect(0, 0, 0, 0),
// Private
m_OldParent(parent)
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
if(m_OldParent)
	{
	m_OldParent->Children->Remove(this);
	m_OldParent->Invalidate(true);
	}
m_OldParent=parent;
if(!parent)
	return;
parent->Children->Append(this);
Invalidate(true);
}

VOID Window::OnVisibleChanged(BOOL visible)
{
FlagHelper::Clear(m_Flags, WindowFlags::Rearrange);
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
