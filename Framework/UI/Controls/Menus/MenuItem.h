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
	VOID KillFocus();
	VOID Open();
	VOID SetFocus();
	Handle<PopupMenu> SubMenu;
	Interactive* Window;

protected:
	// Con-/Destructors
	MenuItem(Interactive* Window, Menu* Menu);

	// Common
	VOID OnKeyDown(Handle<KeyEventArgs> Args);
	Menu* pMenu;
};

}}}