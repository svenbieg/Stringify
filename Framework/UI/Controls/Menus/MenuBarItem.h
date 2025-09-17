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
	using Sentence=Culture::Sentence;

	// Con-/Destructors
	static inline Handle<MenuBarItem> Create(MenuBar* Parent, Handle<Sentence> Label)
		{
		return new MenuBarItem(Parent, Label);
		}

	// Common
	Handle<PopupMenuItem> Add(Handle<Sentence> Label);
	SIZE GetMinSize(RenderTarget* Target)override;
	DynamicHandle<MenuBarItem, Sentence> Label;
	RECT Padding;
	VOID Render(RenderTarget* Target, RECT& Rect)override;

private:
	// Con-/Destructors
	MenuBarItem(MenuBar* Parent, Handle<Sentence> Label);

	// Common
	VOID OnKeyDown(Handle<KeyEventArgs> Args);
	VOID OnLabelChanged(Handle<Sentence> Label);
	VOID OnPointerDown();
	VOID OnPointerEntered();
	VOID OnPointerLeft();
	Handle<String> m_Label;
};

}}}