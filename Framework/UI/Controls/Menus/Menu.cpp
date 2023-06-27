//==========
// Menu.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "UI/Frame.h"
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
if(!GetFlag(uMenuFlags, MenuFlags::KeyboardAccess))
	return false;
CHAR acc=(CHAR)key;
for(auto it=Window->Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto item=Convert<MenuItem>(it->GetCurrent());
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
if(pOpenItem)
	{
	pOpenItem->Close();
	pOpenItem=nullptr;
	}
if(pSelectedItem)
	{
	pSelectedItem->KillFocus();
	pSelectedItem=nullptr;
	}
ClearFlag(uMenuFlags, MenuFlags::KeyboardAccess);
auto frame=Window->GetFrame();
frame->SetCurrentMenu(pParentMenu);
if(pParentMenu)
	{
	auto owner=Convert<MenuItem>(pOwner);
	pParentMenu->Close(owner);
	if(pParentMenu->HasKeyboardAccess())
		pParentMenu->Select();
	}
}

VOID Menu::Close(MenuItem* item)
{
if(pOpenItem==item)
	pOpenItem=nullptr;
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
if(pParentMenu)
	pParentMenu->Exit();
}

BOOL Menu::HasAcceleration()
{
if(!GetFlag(uMenuFlags, MenuFlags::KeyboardAccess))
	return false;
auto frame=Window->GetFrame();
return frame->GetCurrentMenu()==this;
}

BOOL Menu::IsOpen()
{
return pOpenItem!=nullptr;
}

BOOL Menu::IsParentMenu(Menu* menu)
{
if(pParentMenu==menu)
	return true;
auto parent=Convert<Menu>(Window->Parent);
if(parent)
	return parent->IsParentMenu(menu);
return false;
}

VOID Menu::KillKeyboardAccess()
{
ClearFlag(uMenuFlags, MenuFlags::KeyboardAccess);
if(pSelectedItem)
	{
	pSelectedItem->KillFocus();
	pSelectedItem=nullptr;
	}
if(pParentMenu)
	pParentMenu->KillKeyboardAccess();
}

VOID Menu::Open(MenuItem* item)
{
if(pOpenItem==item)
	return;
if(pOpenItem)
	{
	pOpenItem->Close();
	pOpenItem=nullptr;
	}
pOpenItem=item;
if(pOpenItem)
	pOpenItem->Open();
}

VOID Menu::Select()
{
auto item=pSelectedItem;
if(!item)
	{
	auto control=Interactive::GetNextControl(Window, nullptr, 0);
	item=Convert<MenuItem>(control);
	}
Select(item);
}

VOID Menu::Select(MenuItem* item)
{
if(pSelectedItem!=item)
	{
	if(pSelectedItem)
		{
		pSelectedItem->KillFocus();
		pSelectedItem=nullptr;
		}
	pSelectedItem=item;
	}
if(pSelectedItem)
	{
	if(GetFlag(uMenuFlags, MenuFlags::KeyboardAccess))
		pSelectedItem->SetFocus();
	if(IsOpen())
		Open(pSelectedItem);
	}
}

VOID Menu::Switch(MenuItem* item)
{
if(!item)
	return;
if(pOpenItem==item)
	{
	this->Close();
	return;
	}
this->Open(item);
}


//============================
// Con-/Destructors Protected
//============================

Menu::Menu(UI::Window* window, Menu* parent):
Window(window),
pOpenItem(nullptr),
pOwner(nullptr),
pParentMenu(parent),
pSelectedItem(nullptr),
uMenuFlags(MenuFlags::None)
{}

}}}