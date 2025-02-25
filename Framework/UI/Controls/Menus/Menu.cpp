//==========
// Menu.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "UI/Application.h"
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

BOOL Menu::Accelerate(VirtualKey key)
{
if(!FlagHelper::Get(m_MenuFlags, MenuFlags::KeyboardAccess))
	return false;
CHAR acc=(CHAR)key;
for(auto it=m_Panel->Children->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto child=(UI::Window*)it->GetCurrent();
	auto item=dynamic_cast<MenuItem*>(child);
	if(!item)
		continue;
	auto window=item->Window;
	if(!window->Visible||!window->Enabled)
		continue;
	if(item->Accelerator==acc)
		{
		item->Open();
		return true;
		}
	}
return false;
}

VOID Menu::Close()
{
if(m_OpenItem)
	{
	m_OpenItem->Close();
	m_OpenItem=nullptr;
	}
if(m_SelectedItem)
	{
	m_SelectedItem->KillFocus();
	m_SelectedItem=nullptr;
	}
FlagHelper::Clear(m_MenuFlags, MenuFlags::KeyboardAccess);
Application::Get()->SetCurrentMenu(m_ParentMenu);
if(m_ParentMenu)
	{
	m_ParentMenu->m_OpenItem=nullptr;
	if(m_ParentMenu->HasKeyboardAccess())
		m_ParentMenu->Select();
	}
}

VOID Menu::DoKey(KeyEventType type, Handle<KeyEventArgs> args)
{
if(type!=KeyEventType::KeyDown)
	return;
switch(args->Key)
	{
	case VirtualKey::Escape:
		{
		Close();
		break;
		}
	}
args->Handled=true;
}

VOID Menu::Exit()
{
this->Close();
if(m_ParentMenu)
	m_ParentMenu->Exit();
}

BOOL Menu::HasAcceleration()
{
if(!FlagHelper::Get(m_MenuFlags, MenuFlags::KeyboardAccess))
	return false;
return Application::Get()->GetCurrentMenu()==this;
}

BOOL Menu::IsOpen()
{
return m_OpenItem!=nullptr;
}

BOOL Menu::IsParentMenu(Menu* menu)
{
if(!m_ParentMenu)
	return false;
if(m_ParentMenu==menu)
	return true;
return m_ParentMenu->IsParentMenu(menu);
}

VOID Menu::KillKeyboardAccess()
{
FlagHelper::Clear(m_MenuFlags, MenuFlags::KeyboardAccess);
if(m_SelectedItem)
	{
	m_SelectedItem->KillFocus();
	m_SelectedItem=nullptr;
	}
if(m_ParentMenu)
	m_ParentMenu->KillKeyboardAccess();
}

VOID Menu::Open(MenuItem* item)
{
if(m_OpenItem==item)
	return;
if(m_OpenItem)
	{
	m_OpenItem->Close();
	m_OpenItem=nullptr;
	}
m_OpenItem=item;
if(m_OpenItem)
	m_OpenItem->Open();
}

VOID Menu::Select()
{
auto item=m_SelectedItem;
if(!item)
	{
	auto control=Interactive::GetNextControl(m_Panel, nullptr, 0);
	item=dynamic_cast<MenuItem*>(control);
	}
Select(item);
}

VOID Menu::Select(MenuItem* item)
{
if(m_SelectedItem!=item)
	{
	if(m_SelectedItem)
		{
		m_SelectedItem->KillFocus();
		m_SelectedItem=nullptr;
		}
	m_SelectedItem=item;
	}
if(m_SelectedItem)
	{
	if(FlagHelper::Get(m_MenuFlags, MenuFlags::KeyboardAccess))
		m_SelectedItem->SetFocus();
	if(IsOpen())
		Open(m_SelectedItem);
	}
}

VOID Menu::Switch(MenuItem* item)
{
if(!item)
	return;
if(m_OpenItem==item)
	{
	this->Close();
	return;
	}
this->Open(item);
}


//============================
// Con-/Destructors Protected
//============================

Menu::Menu(Menu* parent_menu):
m_MenuFlags(MenuFlags::None),
m_OpenItem(nullptr),
m_Panel(nullptr),
m_ParentMenu(parent_menu),
m_SelectedItem(nullptr)
{}

}}}