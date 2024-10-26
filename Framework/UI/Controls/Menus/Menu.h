//========
// Menu.h
//========

#pragma once


//=======
// Using
//=======

#include "UI/Input/KeyEvent.h"
#include "UI/Input/VirtualKey.h"
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
	Window* GetOwner()const { return m_Owner; }
	Menu* GetParentMenu()const { return m_ParentMenu; }
	BOOL HasAcceleration();
	BOOL HasKeyboardAccess() { return GetFlag(m_MenuFlags, MenuFlags::KeyboardAccess); }
	BOOL IsOpen();
	BOOL IsParentMenu(Menu* Menu);
	VOID KillKeyboardAccess();
	VOID Open(MenuItem* Item);
	Handle<Window> Panel;
	VOID Select();
	VOID Select(MenuItem* Item);
	VOID Switch(MenuItem* Item);

protected:
	// Flags
	enum class MenuFlags
		{
		None=0,
		KeyboardAccess=1
		};

	// Con-/Destructors
	Menu(Window* Owner, Menu* Parent);

	// Common
	MenuFlags m_MenuFlags;
	MenuItem* m_OpenItem;
	Window* m_Owner;
	Menu* m_ParentMenu;
	MenuItem* m_SelectedItem;
};

}}}