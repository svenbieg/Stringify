//==============
// MenuItem.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "PopupMenu.h"

using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//========
// Common
//========

VOID MenuItem::Close()
{
m_Control->KillFocus();
if(SubMenu)
	SubMenu->Close();
}

VOID MenuItem::KillFocus()
{
m_Control->KillFocus();
}

VOID MenuItem::Open()
{
if(!SubMenu)
	{
	m_Control->Clicked(m_Control, nullptr);
	return;
	}
Graphics::RECT rc=m_Control->GetScreenRect();
Graphics::POINT pt(rc.Left, rc.Bottom);
auto parent=m_Control->GetParent();
auto popup=dynamic_cast<PopupMenu*>(parent);
if(popup)
	pt.Set(rc.Right, rc.Top);
SubMenu->Show(pt);
if(m_Menu->HasKeyboardAccess())
	SubMenu->Select();
}

VOID MenuItem::SetFocus()
{
m_Control->SetFocus();
}


//============================
// Con-/Destructors Protected
//============================

MenuItem::MenuItem(Interactive* control, Menu* menu):
Accelerator(0),
m_Control(control),
m_Menu(menu)
{}


//==================
// Common Protected
//==================

VOID MenuItem::OnKeyDown(Handle<KeyEventArgs> args)
{
args->Handled=true;
switch(args->Key)
	{
	case VirtualKey::Escape:
		{
		m_Menu->Close();
		return;
		}
	}
args->Handled=m_Menu->Accelerate(args->Key);
}

}}}