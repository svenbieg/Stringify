//========
// Menu.h
//========

#pragma once


//=======
// Using
//=======

#include "UI/Input/KeyEvent.h"
#include "UI/Input/VirtualKey.h"
#include "UI/Window.h"
#include "MenuItem.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//======
// Menu
//======

class Menu
{
public:
	// Using
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using KeyEventType=UI::Input::KeyEventType;
	using VirtualKey=UI::Input::VirtualKey;

	// Common
	BOOL Accelerate(VirtualKey Key);
	virtual VOID Close();
	VOID Close(MenuItem* Item);
	VOID DoKey(KeyEventType Type, Handle<KeyEventArgs> Args);
	VOID Exit();
	UI::Window* GetOwner()const { return pOwner; }
	Menu* GetParentMenu()const { return pParentMenu; }
	BOOL HasAcceleration();
	BOOL HasKeyboardAccess() { return GetFlag(uMenuFlags, MenuFlags::KeyboardAccess); }
	BOOL IsOpen();
	BOOL IsParentMenu(Menu* Menu);
	VOID KillKeyboardAccess();
	VOID Open(MenuItem* Item);
	VOID Select();
	VOID Select(MenuItem* Item);
	VOID Switch(MenuItem* Item);
	UI::Window* Window;

protected:
	// Flags
	enum class MenuFlags
		{
		None=0,
		KeyboardAccess=1
		};

	// Con-/Destructors
	Menu(UI::Window* Window, Menu* Parent);

	// Common
	MenuItem* pOpenItem;
	UI::Window* pOwner;
	Menu* pParentMenu;
	MenuItem* pSelectedItem;
	MenuFlags uMenuFlags;
};

}}}