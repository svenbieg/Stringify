//===============
// MenuBarItem.h
//===============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Interactive.h"
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

class MenuBar;


//===============
// Menu-Bar-Item
//===============

class MenuBarItem: public Interactive, public MenuItem
{
public:
	// Using
	using KeyEventArgs=UI::Input::KeyEventArgs;

	// Con-/Destructors
	MenuBarItem(MenuBar* MenuBar, Handle<Sentence> Label);

	// Common
	Handle<PopupMenuItem> Add(Handle<Sentence> Label);
	Handle<Brush> GetBackgroundBrush()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	DynamicHandle<MenuBarItem, Sentence> Label;
	RECT Padding;
	VOID Render(RenderTarget* Target, RECT& Rect)override;

private:
	// Common
	VOID OnKeyDown(Handle<KeyEventArgs> Args);
	VOID OnLabelChanged(Handle<Sentence> Label);
	VOID OnPointerDown();
	VOID OnPointerEntered();
	VOID OnPointerLeft();
	Handle<String> hLabel;
};

}}}