//=============
// MenuBar.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "UI/Application.h"
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
m_Entering(false),
m_OldFrame(nullptr)
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
return theme->ControlBrush;
}


//================
// Common Private
//================

VOID MenuBar::OnFrameKeyDown(Handle<KeyEventArgs> args)
{
if(Accelerate(args->Key))
	{
	m_Entering=false;
	args->Handled=true;
	return;
	}
if(args->Key!=VirtualKey::Alt)
	return;
if(m_Entering)
	return;
if(GetFlag(m_MenuFlags, MenuFlags::KeyboardAccess))
	{
	Exit();
	return;
	}
Exit();
SetFlag(m_MenuFlags, MenuFlags::KeyboardAccess);
Application::Current->SetCurrentMenu(this);
m_Entering=true;
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
if(m_Entering)
	{
	m_Entering=false;
	Select();
	}
}

VOID MenuBar::OnParentChanged()
{
auto frame=GetFrame();
if(frame==m_OldFrame)
	return;
if(m_OldFrame)
	m_OldFrame->KeyEvent.Remove(this);
m_OldFrame=frame;
if(frame)
	frame->KeyEvent.Add(this, &MenuBar::OnFrameKeyEvent);
}

}}}