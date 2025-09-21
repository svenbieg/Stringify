//============
// MenuItem.h
//============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Interactive.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//======================
// Forward-Declarations
//======================

class Menu;
class PopupMenu;


//===========
// Menu-Item
//===========

class MenuItem
{
public:
	// Using
	using KeyEventArgs=UI::Input::KeyEventArgs;

	// Common
	CHAR Accelerator;
	VOID Close();
	inline Interactive* GetControl()const { return m_Control; }
	VOID KillFocus();
	VOID Open();
	VOID SetFocus();
	Handle<PopupMenu> SubMenu;

protected:
	// Con-/Destructors
	MenuItem(Interactive* Control, Menu* Menu);

	// Common
	VOID OnKeyDown(Handle<KeyEventArgs> Args);
	Interactive* m_Control;
	Menu* m_Menu;
	};

}}}