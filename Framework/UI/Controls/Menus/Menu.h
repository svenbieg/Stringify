//========
// Menu.h
//========

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Menus/MenuItem.h"
#include "UI/Input/KeyEvent.h"
#include "UI/Input/VirtualKey.h"


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
	MenuItem* Accelerate(VirtualKey Key);
	BOOL Acceleration();
	VOID ClearSelection(MenuItem* Item, FocusReason Reason);
	virtual VOID Close(FocusReason Reason);
	VOID Collapse(FocusReason Reason);
	VOID Escape(FocusReason Reason);
	VOID Exit(FocusReason Reason);
	VOID Expand(FocusReason Reason);
	VOID Expand(MenuItem* Item, FocusReason Reason);
	static inline Menu* GetCurrent() { return s_Current; }
	Window* GetPanel()const { return m_Panel; }
	Menu* GetParentMenu()const { return m_ParentMenu; }
	MenuItem* GetSelected()const { return m_Selected; }
	VOID Invalidate();
	BOOL IsParentMenu(Menu* Menu);
	VOID Select(FocusReason Reason);
	VOID Select(MenuItem* Item, FocusReason Reason);

protected:
	// Flags
	enum class MenuFlags: UINT
		{
		None=0,
		All=7,
		Exit=(1<<0),
		Expand=(1<<1),
		Keyboard=(3<<2),
		KeyboardAccess=(1<<2),
		KeyboardNavigation=(1<<3),
		};

	// Con-/Destructors
	Menu(Menu* ParentMenu, Window* Panel);

	// Common
	MenuFlags m_MenuFlags;
	Window* m_Panel;
	Menu* m_ParentMenu;
	MenuItem* m_Selected;
	static Menu* s_Current;
};

}}}