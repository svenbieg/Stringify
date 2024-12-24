//=================
// Interactive.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "UI/Application.h"
#include "Interactive.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Interactive::Interactive(Window* parent):
Control(parent),
Enabled(this, true),
TabStop(false),
m_InteractiveFlags(InteractiveFlags::None)
{
Enabled.Changed.Add(this, &Interactive::OnEnabledChanged);
Focused.Add(this, &Interactive::OnFocused);
FocusLost.Add(this, &Interactive::OnFocusLost);
KeyDown.Add(this, &Interactive::OnKeyDown);
PointerDown.Add(this, &Interactive::OnPointerDown);
PointerEntered.Add(this, &Interactive::OnPointerEntered);
PointerLeft.Add(this, &Interactive::OnPointerLeft);
PointerUp.Add(this, &Interactive::OnPointerUp);
}


//========
// Common
//========

VOID Interactive::CapturePointer()
{
auto frame=GetFrame();
frame->SetPointerCapture(this);
}

Interactive* Interactive::GetNextControl(Window* window, Interactive* control, INT relative)
{
if(!window)
	{
	if(!control)
		return nullptr;
	window=control->Parent;
	}
Interactive* next=nullptr;
BOOL fwd=(relative>=0);
GetNext(window, control, &next, &relative, fwd, 0);
return next;
}

BOOL Interactive::HasFocus()
{
auto frame=GetFrame();
return frame->GetFocus()==this;
}

BOOL Interactive::HasPointerFocus()
{
return Application::Current->GetPointerFocus()==this;
}

BOOL Interactive::IsCapturingPointer()
{
auto frame=GetFrame();
return frame->GetPointerCapture()==this;
}

BOOL Interactive::IsEnabled()
{
if(!Enabled)
	return false;
Window* parent=Parent;
while(parent)
	{
	auto interactive=Convert<Interactive>(parent);
	if(interactive)
		{
		if(!interactive->Enabled)
			return false;
		}
	parent=parent->Parent;
	}
return true;
}

BOOL Interactive::KillFocus()
{
auto frame=GetFrame();
if(frame->GetFocus()!=this)
	return false;
frame->SetFocus(nullptr);
return true;
}

VOID Interactive::ReleasePointer()
{
auto frame=GetFrame();
if(frame->GetPointerCapture()==this)
	frame->SetPointerCapture(nullptr);
}

VOID Interactive::SetFocus(FocusReason reason)
{
auto frame=GetFrame();
frame->SetFocus(this, reason);
}


//================
// Common Private
//================

BOOL Interactive::GetNext(Window* window, Interactive* control, Interactive** next_ptr, INT* relative_ptr, BOOL fwd, UINT level)
{
auto it=window->Children->First();
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
		if(!child->Visible)
			continue;
		if(!child->IsParentOf(control))
			continue;
		if(GetNext(child, control, next_ptr, relative_ptr, fwd, level+1))
			return true;
		if(!it->Move(fwd, repeat))
			return false;
		break;
		}
	}
else
	{
	if(!fwd)
		it->Last();
	}
INT dir=(fwd? 1: -1);
for(; it->HasCurrent(); it->Move(fwd, repeat))
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	auto interactive=Convert<Interactive>(child);
	if(interactive)
		{
		if(!interactive->Enabled)
			continue;
		if(relative_ptr[0]==0)
			{
			next_ptr[0]=interactive;
			return true;
			}
		relative_ptr[0]-=dir;
		continue;
		}
	if(GetNext(child, nullptr, next_ptr, relative_ptr, fwd, level+1))
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
if(!TabStop)
	return;
if(args->Key!=VirtualKey::Tab)
	return;
auto frame=GetFrame();
BOOL shift=frame->IsKeyDown(VirtualKey::Shift);
INT relative=shift? -1: 1;
Interactive* control=this;
while(control)
	{
	control=GetNextControl(frame, control, relative);
	if(!control)
		break;
	if(control==this)
		break;
	if(control->TabStop)
		{
		control->SetFocus();
		break;
		}
	}
}

VOID Interactive::OnPointerDown(Handle<PointerEventArgs> args)
{
if(args->Button==PointerButton::Left)
	FlagHelper::Set(m_InteractiveFlags, InteractiveFlags::LeftButtonDown);
}

VOID Interactive::OnPointerEntered()
{
auto cursor=this->GetCursor();
auto frame=GetFrame();
frame->SetCursor(cursor);
}

VOID Interactive::OnPointerLeft()
{
auto frame=GetFrame();
frame->SetCursor(nullptr);
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

}}