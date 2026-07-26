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
	// Common
	CHAR Accelerator;
	VOID Collapse(FocusReason Reason);
	VOID Enter(FocusReason Reason);
	VOID Expand(FocusReason Reason);
	inline Interactive* GetControl()const { return m_Control; }
	VOID KillFocus(FocusReason Reason);
	VOID SetFocus(FocusReason Reason);
	Handle<PopupMenu> SubMenu;
	Handle<String> Text;

protected:
	// Con-/Destructors
	MenuItem(Interactive* Control, Menu* Menu);

	// Common
	Interactive* m_Control;
	Menu* m_Menu;
	};

}}}