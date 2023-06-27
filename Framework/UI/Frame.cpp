//===========
// Frame.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Framework/Application.h"
#include "UI/Controls/Menus/MenuBar.h"
#include "UI/Controls/Menus/PopupMenu.h"
#include "UI/Input/Shortcut.h"
#include "Frame.h"

using namespace Framework;
using namespace UI::Controls;
using namespace UI::Controls::Menus;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {


//==================
// Con-/Destructors
//==================

Frame::Frame(Handle<RenderTarget> target, Handle<Theme> theme):
Window(nullptr),
hTarget(target),
hTheme(theme),
pCurrentMenu(nullptr),
pFocus(nullptr),
pOldFocus(nullptr),
pPointerCapture(nullptr),
pPointerFocus(nullptr)
{
Shortcuts=new ShortcutMap();
ZeroMemory(pKeys, 256);
}


//========
// Common
//========

BOOL Frame::DoKey(KeyEventType type, Handle<KeyEventArgs> args)
{
UpdateKeys(type, args->Key);
if(DoShortcut(type, args))
	return true;
KeyEvent(this, type, args);
if(args->Handled)
	return true;
auto focus=Convert<Interactive>(pFocus);
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
	return args->Handled;
	}
if(pCurrentMenu)
	pCurrentMenu->DoKey(type, args);
return args->Handled;
}

VOID Frame::DoPointer(PointerEventType type, Handle<PointerEventArgs> args)
{
POINT pt(args->Point);
if(pPointerCapture)
	{
	POINT const& offset=pPointerCapture->GetFrameOffset();
	args->Point-=offset;
	DoPointer(pPointerCapture, type, args);
	args->Point=pt;
	return;
	}
Interactive* focus=nullptr;
if(pCurrentMenu)
	{
	BOOL inside=DoMenuPointer(type, args, &focus);
	SetPointerFocus(focus);
	if(inside)
		return;
	if(type!=PointerEventType::ButtonDown)
		return;
	}
for(auto it=Children->Last(); it->HasCurrent(); it->MovePrevious())
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
SetPointerFocus(focus);
}

BOOL Frame::DoShortcut(KeyEventType type, Handle<KeyEventArgs> args)
{
if(type!=KeyEventType::KeyDown)
	return false;
BOOL alt=IsKeyDown(VirtualKey::Alt);
BOOL ctrl=IsKeyDown(VirtualKey::Control);
if(ctrl|alt)
	{
	BOOL shift=IsKeyDown(VirtualKey::Shift);
	ShortcutFlags shortcut=(ShortcutFlags)args->Key;
	SetFlag(shortcut, ShortcutFlags::Alt, alt);
	SetFlag(shortcut, ShortcutFlags::Ctrl, ctrl);
	SetFlag(shortcut, ShortcutFlags::Shift, shift);
	auto control=Shortcuts->Get((UINT)shortcut);
	if(control)
		{
		control->Clicked(control, nullptr);
		args->Handled=true;
		return true;
		}
	}
return false;
}

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
if(GetFlag(uFlags, WindowFlags::Rearrange))
	return;
if(rearrange)
	SetFlag(uFlags, WindowFlags::Rearrange);
SetFlag(uFlags, WindowFlags::Repaint);
Invalidated(this);
}

BOOL Frame::IsKeyDown(VirtualKey key)
{
return pKeys[(BYTE)key];
}

VOID Frame::KillFocus()
{
ExitMenu();
SetFocus(nullptr);
SetPointerCapture(nullptr);
ZeroMemory(pKeys, 256);
}

VOID Frame::Rearrange(RenderTarget* target, RECT& rc)
{
auto it=Children->First();
auto content=it->GetCurrent();
if(!content)
	return;
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

VOID Frame::Render(RenderTarget* target)
{
auto rc=GetClientRect();
RenderWindow(this, target, rc, false);
}

VOID Frame::SetCurrentMenu(Menu* menu)
{
if(menu)
	{
	if(!pCurrentMenu)
		{
		pOldFocus=pFocus;
		SetFocus(nullptr);
		}
	}
else
	{
	if(pCurrentMenu)
		{
		SetFocus(pOldFocus);
		pOldFocus=nullptr;
		}
	}
pCurrentMenu=menu;
}

VOID Frame::SetCursor(Handle<Cursor> cursor)
{
CursorChanged(this, cursor);
}

VOID Frame::SetFocus(Interactive* focus, FocusReason reason)
{
if(pFocus==focus)
	return;
if(pFocus)
	pFocus->FocusLost(pFocus);
pFocus=focus;
if(pFocus)
	{
	pFocus->Focused(pFocus, reason);
	auto app=Application::Current;
	app->SetFrame(this);
	}
}

VOID Frame::SetPointerCapture(Interactive* capture)
{
if(capture)
	{
	pPointerCapture=capture;
	PointerCaptured(this);
	}
else
	{
	pPointerCapture=nullptr;
	PointerReleased(this);
	}
}

VOID Frame::SetPointerFocus(Interactive* focus)
{
if(pPointerFocus==focus)
	return;
if(pPointerFocus)
	pPointerFocus->PointerLeft(pPointerFocus);
pPointerFocus=focus;
if(pPointerFocus)
	pPointerFocus->PointerEntered(pPointerFocus);
}

VOID Frame::SetTheme(Handle<Theme> theme)
{
hTheme=theme;
Invalidate(true);
}


//================
// Common Private
//================

BOOL Frame::DoMenuPointer(PointerEventType type, Handle<PointerEventArgs> args, Interactive** focus_ptr)
{
POINT pt(args->Point);
Menu* menu=pCurrentMenu;
BOOL inside=false;
while(menu)
	{
	auto window=menu->Window;
	POINT offset=window->GetFrameOffset();
	args->Point-=offset;
	inside|=DoPointer(window, type, args, focus_ptr);
	args->Point=pt;
	if(args->Handled)
		return true;
	menu=menu->GetParentMenu();
	}
if(type==PointerEventType::ButtonDown)
	{
	if(!inside)
		ExitMenu();
	}
return inside;
}

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
for(auto it=window->Children->Last(); it->HasCurrent(); it->MovePrevious())
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
auto control=Convert<Interactive>(window);
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

VOID Frame::ExitMenu()
{
if(pCurrentMenu)
	{
	pCurrentMenu->Exit();
	pCurrentMenu=nullptr;
	}
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
for(auto it=children->First(); it->HasCurrent(); it->MoveNext())
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

VOID Frame::UpdateKeys(KeyEventType type, VirtualKey key)
{
BYTE set=0;
if(type==KeyEventType::KeyDown)
	set=1;
pKeys[(BYTE)key]=set;
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