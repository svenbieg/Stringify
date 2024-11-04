//=============
// PopupMenu.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/StackPanel.h"
#include "UI/Input/KeyEvent.h"
#include "UI/Popup.h"
#include "Menu.h"
#include "PopupMenuItem.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//======================
// Forward-Declarations
//======================

class MenuBarItem;


//============
// Popup-Menu
//============

class PopupMenu: public Popup, public Menu
{
public:
	// Using
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using KeyEventType=UI::Input::KeyEventType;
	using Menu=UI::Controls::Menus::Menu;
	using Sentence=Culture::Sentence;

	// Con-/Destructors
	PopupMenu(Window* Parent, Menu* ParentMenu=nullptr);

	// Common
	Handle<PopupMenuItem> Add(Handle<Sentence> Label);
	VOID Close()override;
	Handle<Brush> GetBackgroundBrush()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	VOID KillFocus()override;
	Event<PopupMenu> Opened;
	VOID Show(POINT const& Point);
};

}}}