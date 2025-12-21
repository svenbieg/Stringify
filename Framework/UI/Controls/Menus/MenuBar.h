//===========
// MenuBar.h
//===========

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Menus/Menu.h"
#include "UI/Controls/Menus/MenuBarItem.h"
#include "UI/Controls/WrapPanel.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==========
// Menu-Bar
//==========

class MenuBar: public WrapPanel, public Menu
{
public:
	// Using
	using Sentence=Culture::Sentence;

	// Con-/Destructors
	~MenuBar();
	static inline Handle<MenuBar> Create(Window* Parent) { return new MenuBar(Parent); }

	// Common
	Handle<MenuBarItem> Add(Handle<Sentence> Label);

private:
	// Using
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using KeyEventType=UI::Input::KeyEventType;

	// Con-/Destructors
	MenuBar(Window* Parent);

	// Common
	VOID OnFrameKeyEvent(KeyEventType Type, Handle<KeyEventArgs> Args);
	VOID OnFrameKeyDown(Handle<KeyEventArgs> Args);
	VOID OnFrameKeyUp(Handle<KeyEventArgs> Args);
	BOOL m_Entering;
};

}}}