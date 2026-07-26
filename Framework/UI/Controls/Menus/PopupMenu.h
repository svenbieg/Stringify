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

	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<PopupMenu> Create(Menu* Parent=nullptr) { return Object::Create<PopupMenu>(Parent); }

	// Common
	Handle<PopupMenuItem> Add(Handle<Sentence> Label);
	VOID Close(FocusReason Reason)override;
	SIZE GetMinSize(RenderTarget* Target)override;
	Handle<Popup> GetPopup()const { return m_Popup; }
	Event<PopupMenu> Opened;
	VOID Show(POINT const& Point, FocusReason Reason);

protected:
	// Con-/Destructors
	PopupMenu(Menu* Parent);

private:
	// Common
	VOID OnPopupFocusLost(Frame* Sender, FocusReason Reason, Frame* Active);
	Handle<Popup> m_Popup;
};

}}}