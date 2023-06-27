//=============
// PopupMenu.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/StackPanel.h"
#include "UI/Input/KeyEvent.h"
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

class PopupMenu: public StackPanel, public Menu
{
public:
	// Using
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using KeyEventType=UI::Input::KeyEventType;

	// Con-/Destructors
	PopupMenu(UI::Frame* Frame, Menu* Parent);

	// Common
	Handle<PopupMenuItem> Add(Handle<Sentence> Label);
	VOID Close()override;
	Handle<Brush> GetBackgroundBrush()override;
	Handle<Brush> GetBorderBrush()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	Event<PopupMenu> Popup;
	VOID Show(UI::Window* Owner, POINT const& Point);
};

}}}