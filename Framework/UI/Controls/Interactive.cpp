//=================
// Interactive.cpp
//=================

#include "Interactive.h"


//=======
// Using
//=======

#include "UI/Application.h"
#include "UI/Frame.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Interactive::~Interactive()
{
if(m_Frame)
	{
	if(m_Frame->m_Focus==this)
		m_Frame->m_Focus=nullptr;
	if(m_Frame->m_PointerCapture==this)
		m_Frame->m_PointerCapture=nullptr;
	}
if(s_PointerFocus==this)
	s_PointerFocus=nullptr;
}


//========
// Common
//========

VOID Interactive::CapturePointer()
{
m_Frame->SetPointerCapture(this);
}

Handle<Cursor> Interactive::GetCursor()
{
return m_Theme->DefaultCursor;
}

Interactive* Interactive::GetNextControl(Window* window, Interactive* control, BOOL fwd)
{
Interactive* next=nullptr;
GetNext(window, control, &next, fwd, 0);
return next;
}

BOOL Interactive::HasFocus()
{
if(!m_Frame)
	return false;
return m_Frame->GetFocus()==this;
}

BOOL Interactive::HasPointerFocus()
{
return s_PointerFocus==this;
}

BOOL Interactive::IsCapturingPointer()
{
if(!m_Frame)
	return false;
return m_Frame->GetPointerCapture()==this;
}

BOOL Interactive::IsEnabled()
{
if(!Enabled)
	return false;
Window* parent=m_Parent;
while(parent)
	{
	auto interactive=dynamic_cast<Interactive*>(parent);
	if(interactive)
		{
		if(!interactive->Enabled)
			return false;
		}
	parent=parent->GetParent();
	}
return true;
}

BOOL Interactive::KillFocus(FocusReason reason)
{
if(!m_Frame)
	return false;
auto focus=m_Frame->GetFocus();
if(focus!=this)
	return false;
m_Frame->SetFocus(nullptr, reason);
return true;
}

VOID Interactive::ReleasePointer()
{
if(m_Frame->GetPointerCapture()==this)
	m_Frame->SetPointerCapture(nullptr);
}

VOID Interactive::SetFocus(FocusReason reason)
{
if(m_Frame)
	m_Frame->SetFocus(this, reason);
}

VOID Interactive::SetPointerFocus(Interactive* focus)
{
if(s_PointerFocus==focus)
	return;
auto old_focus=s_PointerFocus;
s_PointerFocus=focus;
if(old_focus)
	old_focus->PointerLeft(old_focus);
if(focus)
	focus->PointerEntered(focus);
}


//============================
// Con-/Destructors Protected
//============================

Interactive::Interactive(Window* parent):
Control(parent),
Enabled(this, true),
TabStop(false),
m_InteractiveFlags(InteractiveFlags::None),
m_KeyDown(VirtualKey::None)
{
Enabled.Changed.Add(this, &Interactive::OnEnabledChanged);
Focused.Add(this, &Interactive::OnFocused);
FocusLost.Add(this, &Interactive::OnFocusLost);
KeyDown.Add(this, &Interactive::OnKeyDown);
KeyPressed.Add(this, &Interactive::OnKeyPressed);
KeyUp.Add(this, &Interactive::OnKeyUp);
PointerDown.Add(this, &Interactive::OnPointerDown);
PointerEntered.Add(this, &Interactive::OnPointerEntered);
PointerLeft.Add(this, &Interactive::OnPointerLeft);
PointerUp.Add(this, &Interactive::OnPointerUp);
}


//================
// Common Private
//================

BOOL Interactive::GetNext(Window* window, Interactive* control, Interactive** next_ptr, BOOL fwd, UINT level)
{
auto it=window->Children->Begin();
if(!it->HasCurrent())
	return false;
BOOL repeat=(level==0);
if(control)
	{
	for(; it->HasCurrent(); it->MoveNext())
		{
		auto child=it->GetCurrent();
		if(child==control)
			break;
		if(child->IsParentOf(control))
			{
			if(GetNext(child, control, next_ptr, fwd, level+1))
				return true;
			break;
			}
		}
	if(!it->Move(fwd, repeat))
		return false;
	}
else
	{
	if(!fwd)
		it->End();
	}
for(; it->HasCurrent(); it->Move(fwd, repeat))
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	auto interactive=child.As<Interactive>();
	if(interactive)
		{
		if(!interactive->Enabled)
			continue;
		if(!interactive->TabStop)
			continue;
		*next_ptr=interactive;
		return true;
		}
	if(GetNext(child, nullptr, next_ptr, fwd, level+1))
		return true;
	}
return false;
}

VOID Interactive::OnEnabledChanged()
{
Invalidate();
}

VOID Interactive::OnFocused()
{
Invalidate();
}

VOID Interactive::OnFocusLost()
{
Invalidate();
}

VOID Interactive::OnKeyDown(Handle<KeyEventArgs> args)
{
m_KeyDown=args->Key;
}

VOID Interactive::OnKeyPressed(Handle<KeyEventArgs> args)
{
switch(args->Key)
	{
	case VirtualKey::Return:
		{
		Clicked(this, nullptr);
		break;
		}
	default:
		{
		return;
		}
	}
args->Handled=true;
}

VOID Interactive::OnKeyUp(Handle<KeyEventArgs> args)
{
if(m_KeyDown==args->Key)
	{
	m_KeyDown=VirtualKey::None;
	KeyPressed(this, args);
	}
}

VOID Interactive::OnPointerDown(Handle<PointerEventArgs> args)
{
if(args->Button==PointerButton::Left)
	FlagHelper::Set(m_InteractiveFlags, InteractiveFlags::LeftButtonDown);
}

VOID Interactive::OnPointerEntered()
{
auto cursor=GetCursor();
m_Frame->SetCursor(cursor);
Invalidate();
}

VOID Interactive::OnPointerLeft()
{
if(!m_Frame)
	return;
auto cursor=m_Theme->DefaultCursor;
m_Frame->SetCursor(cursor);
Invalidate();
}

VOID Interactive::OnPointerUp(Handle<PointerEventArgs> args)
{
if(args->Button!=PointerButton::Left)
	return;
BOOL clicked=FlagHelper::Get(m_InteractiveFlags, InteractiveFlags::LeftButtonDown);
FlagHelper::Clear(m_InteractiveFlags, InteractiveFlags::LeftButtonDown);
if(clicked)
	Clicked(this, args);
}

Interactive* Interactive::s_PointerFocus=nullptr;

}}