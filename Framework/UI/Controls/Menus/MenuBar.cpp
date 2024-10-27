//=============
// MenuBar.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "UI/Frame.h"
#include "MenuBar.h"
#include "PopupMenu.h"

using namespace Graphics;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

MenuBar::MenuBar(UI::Window* parent):
WrapPanel(parent),
Menu(nullptr),
bEntering(false),
pOldFrame(nullptr)
{
Padding.Set(2, 0, 2, 0);
m_Panel=this;
Parent.Changed.Add(this, &MenuBar::OnParentChanged);
OnParentChanged();
}

MenuBar::~MenuBar()
{
auto frame=GetFrame();
frame->KeyEvent.Remove(this);
}


//========
// Common
//========

Handle<MenuBarItem> MenuBar::Add(Handle<Sentence> label)
{
return new MenuBarItem(this, label);
}

Handle<Brush> MenuBar::GetBackgroundBrush()
{
auto theme=GetTheme();
return theme->GetControlBrush();
}


//================
// Common Private
//================

VOID MenuBar::OnFrameKeyDown(Handle<KeyEventArgs> args)
{
if(Accelerate(args->Key))
	{
	bEntering=false;
	args->Handled=true;
	return;
	}
if(args->Key!=VirtualKey::Alt)
	return;
if(bEntering)
	return;
if(GetFlag(m_MenuFlags, MenuFlags::KeyboardAccess))
	{
	Exit();
	return;
	}
Exit();
SetFlag(m_MenuFlags, MenuFlags::KeyboardAccess);
Application::Current->SetCurrentMenu(this);
bEntering=true;
Invalidate();
args->Handled=true;
}

VOID MenuBar::OnFrameKeyEvent(KeyEventType type, Handle<KeyEventArgs> args)
{
switch(type)
	{
	case KeyEventType::KeyDown:
		{
		OnFrameKeyDown(args);
		break;
		}
	case KeyEventType::KeyUp:
		{
		OnFrameKeyUp(args);
		break;
		}
	}
}

VOID MenuBar::OnFrameKeyUp(Handle<KeyEventArgs> args)
{
if(args->Key!=VirtualKey::Alt)
	return;
args->Handled=true;
if(bEntering)
	{
	bEntering=false;
	Select();
	}
}

VOID MenuBar::OnParentChanged()
{
auto frame=GetFrame();
if(frame==pOldFrame)
	return;
if(pOldFrame)
	pOldFrame->KeyEvent.Remove(this);
pOldFrame=frame;
if(frame)
	frame->KeyEvent.Add(this, &MenuBar::OnFrameKeyEvent);
}

}}}