//=============
// PopupMenu.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Menus/Menu.h"
#include "UI/Controls/Menus/PopupMenuItem.h"
#include "UI/Controls/StackPanel.h"
#include "UI/Input/KeyEvent.h"
#include "UI/Popup.h"


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
	using Sentence=Culture::Sentence;

	// Con-/Destructors
	static inline Handle<PopupMenu> Create(Menu* Parent=nullptr) { return new PopupMenu(Parent); }

	// Common
	Handle<PopupMenuItem> Add(Handle<Sentence> Label);
	VOID Close()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	Event<PopupMenu> Opened;
	VOID Show(POINT const& Point);

protected:
	// Con-/Destructors
	PopupMenu(Menu* Parent);

private:
	// Common
	VOID OnPopupLostFocus();
	Handle<Popup> m_Popup;
};

}}}