//==============
// MenuItem.cpp
//==============

#include "MenuItem.h"


//=======
// Using
//=======

#include "UI/Controls/Menus/PopupMenu.h"

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

VOID MenuItem::Collapse(FocusReason reason)
{
if(SubMenu)
	SubMenu->Collapse(reason);
}

VOID MenuItem::Enter(FocusReason reason)
{
if(SubMenu)
	{
	Expand(reason);
	}
else
	{
	m_Control->Clicked(m_Control, nullptr);
	}
}

VOID MenuItem::Expand(FocusReason reason)
{
if(!SubMenu)
	return;
Graphics::RECT rc=m_Control->GetScreenRect();
Graphics::POINT pt(rc.Left, rc.Bottom);
auto parent=m_Control->GetParent();
auto popup=dynamic_cast<PopupMenu*>(parent);
if(popup)
	pt.Set(rc.Right, rc.Top);
SubMenu->Show(pt, reason);
}

VOID MenuItem::KillFocus(FocusReason reason)
{
m_Control->KillFocus(reason);
}

VOID MenuItem::SetFocus(FocusReason reason)
{
m_Control->SetFocus(reason);
}


//============================
// Con-/Destructors Protected
//============================

MenuItem::MenuItem(Interactive* control, Menu* menu):
Accelerator(0),
m_Control(control),
m_Menu(menu)
{}

}}}