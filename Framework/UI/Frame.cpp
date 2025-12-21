//===========
// Frame.cpp
//===========

#include "Frame.h"


//=======
// Using
//=======

#include "UI/Controls/Input.h"
#include "UI/Controls/Menus/MenuBar.h"
#include "UI/Controls/Menus/PopupMenu.h"
#include "UI/Application.h"

using namespace Graphics;
using namespace UI::Controls;
using namespace UI::Controls::Menus;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {


//========
// Common
//========

Graphics::SIZE Frame::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
auto content=GetVisibleChild(0);
if(content)
	size=content->GetMinSize(target);
FLOAT scale=GetScaleFactor();
return size.Max(MinSize*scale);
}

VOID Frame::Invalidate(BOOL rearrange)
{
if(rearrange)
	FlagHelper::Set(m_Flags, WindowFlags::Rearrange);
FlagHelper::Set(m_Flags, WindowFlags::Repaint);
Invalidated(this);
}

BOOL Frame::IsKeyDown(VirtualKey key)
{
return m_Keys[(BYTE)key];
}

VOID Frame::KillFocus()
{
SetFocus(nullptr);
SetPointerCapture(nullptr);
MemoryHelper::Fill(m_Keys, sizeof(m_Keys), 0);
}

VOID Frame::Rearrange(RenderTarget* target, RECT& rc)
{
auto it=Children->Begin();
if(!it->HasCurrent())
	return;
auto content=it->GetCurrent();
content->Move(target, rc);
for(it->MoveNext(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	RECT rc_child=child->GetRect();
	SIZE min_size=child->GetMinSize(target);
	rc_child.SetSize(min_size);
	rc_child.SetParent(rc);
	child->Move(target, rc_child);
	}
}

VOID Frame::SetFocus(Interactive* focus, FocusReason reason)
{
if(m_Focus==focus)
	return;
if(m_Focus)
	m_Focus->FocusLost(m_Focus);
m_Focus=focus;
if(m_Focus)
	m_Focus->Focused(m_Focus, reason);
}


//============================
// Con-/Destructors Protected
//============================

Frame::Frame():
Window(nullptr),
// Protected
m_PointerCapture(nullptr),
// Private
m_Focus(nullptr)
{
m_Frame=this;
MemoryHelper::Fill(m_Keys, sizeof(m_Keys), 0);
}


//==================
// Common Protected
//==================

BOOL Frame::DoKey(KeyEventType type, Handle<KeyEventArgs> args)
{
UpdateKeys(type, args->Key);
FlagHelper::Set(args->Flags, KeyEventFlags::Alt, IsKeyDown(VirtualKey::Alt));
FlagHelper::Set(args->Flags, KeyEventFlags::Ctrl, IsKeyDown(VirtualKey::Control));
FlagHelper::Set(args->Flags, KeyEventFlags::Shift, IsKeyDown(VirtualKey::Shift));
if(type==KeyEventType::KeyDown)
	{
	if(Application::GetCurrent()->Shortcut(args))
		return true;
	}
KeyEvent(this, type, args);
if(args->Handled)
	return true;
auto focus=dynamic_cast<Interactive*>(m_Focus);
if(focus)
	{
	switch(type)
		{
		case KeyEventType::KeyDown:
			{
			focus->KeyDown(focus, args);
			break;
			}
		case KeyEventType::KeyUp:
			{
			focus->KeyUp(focus, args);
			break;
			}
		}
	}
return args->Handled;
}

VOID Frame::DoPointer(PointerEventType type, Handle<PointerEventArgs> args)
{
POINT pt(args->Point);
if(m_PointerCapture)
	{
	POINT const& offset=m_PointerCapture->GetFrameOffset();
	args->Point-=offset;
	DoPointer(m_PointerCapture, type, args);
	args->Point=pt;
	return;
	}
Interactive* focus=nullptr;
for(auto it=Children->End(); it->HasCurrent(); it->MovePrevious())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	RECT const& child_rc=child->GetRect();
	if(!pt.Inside(child_rc))
		continue;
	POINT pt_child(pt);
	pt_child.Left-=child_rc.Left;
	pt_child.Top-=child_rc.Top;
	args->Point=pt_child;
	DoPointer(child, type, args, &focus);
	args->Point=pt;
	if(args->Handled)
		break;
	}
Application::GetCurrent()->SetPointerFocus(focus);
}

VOID Frame::RenderWindow(Window* window, RenderTarget* target, RECT const& rc, BOOL override)
{
BOOL repaint=window->IsInvalidated();
repaint|=override;
if(repaint)
	{
	RECT rc_wnd=window->GetFrameRect();
	RECT rc_clip(rc_wnd);
	rc_clip.SetBounds(rc);
	if(rc_clip)
		{
		POINT offset(rc_wnd.Left, rc_wnd.Top);
		target->Clip(offset, rc_clip);
		RECT rc_client(0, 0, rc_clip.GetWidth(), rc_clip.GetHeight());
		window->Render(target, rc_client);
		window->Rendered(window, target, rc_client);
		target->Unclip();
		window->Validate();
		override=true;
		}
	}
auto children=window->Children;
for(auto it=children->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	RECT rc_child=child->GetFrameRect();
	rc_child.SetBounds(rc);
	if(!rc_child)
		continue;
	RenderWindow(child, target, rc_child, override);
	}
}


//================
// Common Private
//================

VOID Frame::DoPointer(Interactive* control, PointerEventType type, Handle<PointerEventArgs> args)
{
switch(type)
	{
	case PointerEventType::ButtonDown:
		{
		control->PointerDown(control, args);
		break;
		}
	case PointerEventType::ButtonUp:
		{
		control->PointerUp(control, args);
		break;
		}
	case PointerEventType::Move:
		{
		control->PointerMoved(control, args);
		break;
		}
	case PointerEventType::Wheel:
		{
		control->PointerWheel(control, args);
		break;
		}
	}
}

BOOL Frame::DoPointer(Window* window, PointerEventType type, Handle<PointerEventArgs> args, Interactive** focus_ptr)
{
POINT pt(args->Point);
BOOL inside=false;
for(auto it=window->Children->End(); it->HasCurrent(); it->MovePrevious())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	RECT const& child_rc=child->GetRect();
	if(!pt.Inside(child_rc))
		continue;
	inside=true;
	POINT pt_child(pt);
	pt_child.Left-=child_rc.Left;
	pt_child.Top-=child_rc.Top;
	args->Point=pt_child;
	DoPointer(child, type, args, focus_ptr);
	args->Point=pt;
	if(args->Handled)
		return true;
	}
auto control=dynamic_cast<Interactive*>(window);
if(control)
	{
	if(control->IsEnabled())
		{
		if(!*focus_ptr)
			*focus_ptr=control;
		DoPointer(control, type, args);
		}
	}
return inside;
}

VOID Frame::UpdateKeys(KeyEventType type, VirtualKey key)
{
BYTE set=0;
if(type==KeyEventType::KeyDown)
	set=1;
m_Keys[(BYTE)key]=set;
}

VOID Frame::UpdateKeys(PointerEventType type, PointerButton button)
{
VirtualKey key=VirtualKey::None;
switch(button)
	{
	case PointerButton::Left:
		{
		key=VirtualKey::LButton;
		break;
		}
	case PointerButton::Right:
		{
		key=VirtualKey::RButton;
		break;
		}
	case PointerButton::Wheel:
		{
		key=VirtualKey::MButton;
		break;
		}
	default:
		{
		break;
		}
	}
switch(type)
	{
	case PointerEventType::ButtonDown:
		{
		UpdateKeys(KeyEventType::KeyDown, key);
		break;
		}
	case PointerEventType::ButtonUp:
		{
		UpdateKeys(KeyEventType::KeyUp, key);
		break;
		}
	default:
		{
		break;
		}
	}
}

}