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
Window->KillFocus();
if(SubMenu)
	SubMenu->Close();
}

VOID MenuItem::KillFocus()
{
Window->KillFocus();
}

VOID MenuItem::Open()
{
if(!SubMenu)
	{
	Window->Clicked(Window, nullptr);
	return;
	}
Graphics::RECT rc=Window->GetFrameRect();
Graphics::POINT pt(rc.Left, rc.Bottom);
auto popup=Convert<PopupMenu>(Window->Parent);
if(popup)
	pt.Set(rc.Right, rc.Top);
SubMenu->Show(pt);
if(pMenu->HasKeyboardAccess())
	SubMenu->Select();
}

VOID MenuItem::SetFocus()
{
Window->SetFocus();
}


//============================
// Con-/Destructors Protected
//============================

MenuItem::MenuItem(Interactive* window, Menu* menu):
Accelerator(0),
Window(window),
pMenu(menu)
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
		pMenu->Close();
		return;
		}
	}
args->Handled=pMenu->Accelerate(args->Key);
}

}}}