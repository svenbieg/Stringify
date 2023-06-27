//===========
// MenuBar.h
//===========

#pragma once


//=======
// Using
//=======

#include "UI/Controls/WrapPanel.h"
#include "Menu.h"
#include "MenuBarItem.h"


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
	// Con-/Destructors
	MenuBar(UI::Window* Parent);
	~MenuBar();

	// Common
	Handle<MenuBarItem> Add(Handle<Sentence> Label);
	Handle<Brush> GetBackgroundBrush()override;

private:
	// Using
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using KeyEventType=UI::Input::KeyEventType;

	// Common
	VOID OnFrameKeyEvent(KeyEventType Type, Handle<KeyEventArgs> Args);
	VOID OnFrameKeyDown(Handle<KeyEventArgs> Args);
	VOID OnFrameKeyUp(Handle<KeyEventArgs> Args);
	VOID OnParentChanged();
	BOOL bEntering;
	Frame* pOldFrame;
};

}}}