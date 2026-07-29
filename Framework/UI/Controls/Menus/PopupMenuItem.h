//=================
// PopupMenuItem.h
//=================

#pragma once


//=======
// Using
//=======

#include "Culture/Sentence.h"
#include "Graphics/Icon.h"
#include "Timing/Timer.h"
#include "UI/Controls/Menus/MenuItem.h"
#include "UI/Controls/Interactive.h"
#include "UI/Controls/TextBlock.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//======================
// Forward-Declarations
//======================

class PopupMenu;


//=================
// Popup-Menu-Item
//=================

class PopupMenuItem: public Interactive, public MenuItem
{
public:
	// Using
	using Bitmap=Graphics::Bitmap;
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using Sentence=Culture::Sentence;
	using Timer=Timing::Timer;

	// Friends
	friend Object;
	friend PopupMenu;

	// Con-/Destructors
	static inline Handle<PopupMenuItem> Create(PopupMenu* Parent, Handle<Sentence> Label)
		{
		return Object::Create<PopupMenuItem>(Parent, Label);
		}

	// Common
	Handle<PopupMenuItem> Add(Handle<Sentence> Label);
	Property<PopupMenuItem, BOOL> Checked;
	Event<PopupMenuItem> Clicked;
	Handle<Brush> GetBackground()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	Handle<Brush> Highlight;
	Handle<Graphics::Icon> Icon;
	BOOL IsSeparator() { return Text==nullptr; }
	DynamicHandle<PopupMenuItem, Sentence> Label;
	RECT Padding;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	VOID SetColumns(UINT SymbolWidth, UINT LabelWidth, UINT ShortcutWidth);
	Handle<String> Shortcut;
	Handle<String> Symbol;

private:
	// Settings
	static const UINT SHORTCUT_PADDING=12;
	static const UINT SYMBOL_PADDING=12;

	// Con-/Destructors
	PopupMenuItem(PopupMenu* Parent, Handle<Sentence> Label);

	// Common
	VOID OnCheckedChanged(BOOL Checked);
	VOID OnFocused(Interactive* Previous, FocusReason Reason);
	VOID OnInteractiveClicked();
	VOID OnLabelChanged(Handle<Sentence> Label);
	VOID OnKeyDown(Handle<KeyEventArgs> Args);
	VOID OnKeyPressed(Handle<KeyEventArgs> Args);
	VOID OnPointerDown();
	VOID OnPointerEntered();
	VOID OnPointerLeft();
	VOID OnTimerTriggered();
	Handle<Bitmap> m_Icon;
	Handle<Bitmap> m_IconDisabled;
	UINT m_LabelWidth;
	UINT m_ShortcutWidth;
	UINT m_SymbolWidth;
	Handle<Timer> m_Timer;
};

}}}
