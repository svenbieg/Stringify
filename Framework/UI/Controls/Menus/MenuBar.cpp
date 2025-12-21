//=============
// MenuBar.cpp
//=============

#include "MenuBar.h"


//=======
// Using
//=======

#include "UI/Controls/Menus/PopupMenu.h"
#include "UI/Application.h"
#include "UI/Frame.h"

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
return MenuBarItem::Create(this, label);
}


//==========================
// Con-/Destructors Private
//==========================

MenuBar::MenuBar(Window* parent):
WrapPanel(parent),
Menu(nullptr),
m_Entering(false)
{
Padding.Set(2, 0, 2, 0);
m_Panel=this;
auto frame=GetFrame();
frame->KeyEvent.Add(this, &MenuBar::OnFrameKeyEvent);
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
if(FlagHelper::Get(m_MenuFlags, MenuFlags::KeyboardAccess))
	{
	Exit();
	return;
	}
Exit();
FlagHelper::Set(m_MenuFlags, MenuFlags::KeyboardAccess);
Application::GetCurrent()->SetCurrentMenu(this);
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

}}}