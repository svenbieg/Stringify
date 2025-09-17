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
if(!m_Parent)
	return;
auto children=m_Parent->Children;
children->Remove(this, false);
children->Append(this, false);
Invalidate(true);
}

Handle<Brush> Window::GetBackground()
{
return nullptr;
}

Graphics::RECT Window::GetClientRect()const
{
return RECT(0, 0, m_Rect.GetWidth(), m_Rect.GetHeight());
}

Graphics::POINT Window::GetFrameOffset()const
{
POINT pt(m_Rect.Left, m_Rect.Top);
if(m_Parent)
	pt+=m_Parent->GetFrameOffset();
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
for(auto it=Children->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	SIZE child_size=child->GetMinSize(target);
	auto control=child.As<Control>();
	if(control)
		child_size.AddPadding(control->Margin*scale);
	size.Width=TypeHelper::Max(size.Width, child_size.Width);
	size.Height=TypeHelper::Max(size.Height, child_size.Height);
	}
return size.Max(MinSize*scale);
}

FLOAT Window::GetScaleFactor()const
{
if(!m_Parent)
	return Scale;
FLOAT scale=m_Parent->GetScaleFactor();
return Scale*scale;
}

Graphics::POINT Window::GetScreenOffset()const
{
POINT pt(m_Rect.Left, m_Rect.Top);
if(m_Parent)
	pt+=m_Parent->GetScreenOffset();
return pt;
}

Graphics::RECT Window::GetScreenRect()const
{
POINT offset=GetScreenOffset();
return m_Rect.SetPosition(offset);
}

Handle<RenderTarget> Window::GetTarget()
{
auto frame=this->GetFrame();
if(!frame)
	return nullptr;
return frame->GetTarget();
}

Handle<Window> Window::GetVisibleChild(UINT id)
{
for(auto it=Children->Begin(); it->HasCurrent(); it->MoveNext())
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
if(m_Parent)
	{
	transparent=true;
	auto background=GetBackground();
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
	m_Parent->Invalidate(rearrange);
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
Window* parent=window->m_Parent;
while(parent)
	{
	if(parent==this)
		return true;
	parent=parent->m_Parent;
	}
return false;
}

BOOL Window::IsVisible()
{
if(!Visible)
	return false;
if(!m_Parent)
	return true;
return m_Parent->IsVisible();
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
auto background=GetBackground();
if(!background)
	return;
RECT rc_fill(rc);
auto offset=target->GetOffset();
rc_fill.Move(offset);
target->FillRect(rc_fill, background);
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
Scale(1.f),
Visible(this, true),
// Protected
m_Flags(WindowFlags::None),
m_Frame(nullptr),
m_Rect(0, 0, 0, 0),
// Private
m_Parent(parent)
{
Children=ChildList::Create();
Visible.Changed.Add(this, &Window::OnVisibleChanged);
if(m_Parent)
	{
	m_Parent->Children->Append(this);
	m_Frame=m_Parent->m_Frame;
	m_Theme=m_Parent->m_Theme;
	}
}


//================
// Common Private
//================

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
	if(m_Parent)
		m_Parent->Invalidate(true);
	}
}

}