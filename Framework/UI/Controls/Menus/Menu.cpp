//==========
// Menu.cpp
//==========

#include "Menu.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//========
// Common
//========

MenuItem* Menu::Accelerate(VirtualKey key)
{
if(!Acceleration())
	return nullptr;
CHAR acc=(CHAR)key;
for(auto it=m_Panel->Children->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto child=(UI::Window*)it->GetCurrent();
	auto item=dynamic_cast<MenuItem*>(child);
	if(!item)
		continue;
	auto control=item->GetControl();
	if(!control->Visible||!control->Enabled)
		continue;
	if(item->Accelerator==acc)
		return item;
	}
return nullptr;
}

BOOL Menu::Acceleration()
{
if(!FlagHelper::Get(m_MenuFlags, MenuFlags::Keyboard))
	return false;
return s_Current==this;
}

VOID Menu::ClearSelection(MenuItem* item, FocusReason reason)
{
if(m_Selected!=item)
	return;
FlagHelper::Clear(m_MenuFlags, MenuFlags::Keyboard);
m_Selected=nullptr;
m_Panel->Invalidate();
}

VOID Menu::Close(FocusReason reason)
{
FlagHelper::Clear(m_MenuFlags, MenuFlags::All);
m_Panel->Invalidate();
if(m_Selected)
	{
	m_Selected->Collapse(reason);
	m_Selected->KillFocus(reason);
	m_Selected=nullptr;
	}
}

VOID Menu::Collapse(FocusReason reason)
{
FlagHelper::Clear(m_MenuFlags, MenuFlags::Expand);
Select(reason);
}

VOID Menu::Escape(FocusReason reason)
{
if(m_ParentMenu)
	{
	m_ParentMenu->Collapse(reason);
	}
else
	{
	Close(reason);
	}
}

VOID Menu::Exit(FocusReason reason)
{
Close(reason);
FlagHelper::Set(m_MenuFlags, MenuFlags::Exit);
if(m_ParentMenu)
	m_ParentMenu->Exit(reason);
}

VOID Menu::Expand(FocusReason reason)
{
FlagHelper::Set(m_MenuFlags, MenuFlags::Expand);
Select(reason);
}

VOID Menu::Expand(MenuItem* item, FocusReason reason)
{
BOOL exit=FlagHelper::Clear(m_MenuFlags, MenuFlags::Exit);
if(exit)
	return;
FlagHelper::Set(m_MenuFlags, MenuFlags::Expand);
Select(item, reason);
}

BOOL Menu::IsParentMenu(Menu* menu)
{
if(!m_ParentMenu)
	return false;
if(m_ParentMenu==menu)
	return true;
return m_ParentMenu->IsParentMenu(menu);
}

VOID Menu::Select(FocusReason reason)
{
auto item=m_Selected;
if(!item)
	{
	auto next=Interactive::GetNextControl(m_Panel, nullptr);
	item=dynamic_cast<MenuItem*>(next);
	}
Select(item, reason);
}

VOID Menu::Select(MenuItem* item, FocusReason reason)
{
auto old_menu=s_Current;
s_Current=this;
m_Selected=item;
FlagHelper::Clear(m_MenuFlags, MenuFlags::Exit);
if(reason==FocusReason::Keyboard)
	FlagHelper::Set(m_MenuFlags, MenuFlags::KeyboardNavigation);
m_Panel->Invalidate();
if(m_Selected)
	{
	if(reason==FocusReason::Keyboard)
		m_Selected->SetFocus(reason);
	if(FlagHelper::Get(m_MenuFlags, MenuFlags::Expand))
		{
		if(m_Selected->SubMenu)
			{
			m_Selected->Expand(reason);
			}
		else
			{
			FlagHelper::Clear(m_MenuFlags, MenuFlags::Expand);
			}
		}
	}
while(old_menu)
	{
	if(s_Current==old_menu)
		break;
	if(s_Current)
		{
		if(s_Current->IsParentMenu(old_menu))
			break;
		}
	old_menu->Close(FocusReason::None);
	old_menu=old_menu->GetParentMenu();
	}
}


//============================
// Con-/Destructors Protected
//============================

Menu::Menu(Menu* parent_menu, Window* panel):
m_MenuFlags(MenuFlags::None),
m_Panel(panel),
m_ParentMenu(parent_menu),
m_Selected(nullptr)
{}


//==================
// Common Protected
//==================

Menu* Menu::s_Current=nullptr;

}}}