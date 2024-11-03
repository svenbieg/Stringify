//=================
// PopupMenuItem.h
//=================

#pragma once


//=======
// Using
//=======

#include "Devices/Timers/Timer.h"
#include "Graphics/Icon.h"
#include "UI/Controls/Interactive.h"
#include "UI/Controls/TextBlock.h"
#include "MenuItem.h"
#include "Sentence.h"


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
	using Timer=Devices::Timers::Timer;

	// Con-/Destructors
	PopupMenuItem(PopupMenu* PopupMenu, Handle<Sentence> Label);

	// Common
	Event<PopupMenuItem> Clicked;
	Handle<Brush> GetBackgroundBrush()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	Handle<Graphics::Icon> Icon;
	BOOL IsSeparator() { return Text==nullptr; }
	DynamicHandle<PopupMenuItem, Sentence> Label;
	RECT Padding;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	VOID SetColumns(UINT IconWidth, UINT LabelWidth, UINT ShortcutWidth);
	Handle<String> Shortcut;
	Handle<String> Text;

private:
	// Common
	VOID DoClick();
	VOID OnClicked();
	VOID OnLabelChanged(Handle<Sentence> Label);
	VOID OnKeyDown(Handle<KeyEventArgs> Args);
	VOID OnPointerDown();
	VOID OnPointerEntered();
	VOID OnPointerLeft();
	VOID OnTimerTriggered();
	Handle<Bitmap> m_Icon;
	Handle<Bitmap> m_IconDisabled;
	UINT m_IconWidth;
	UINT m_LabelWidth;
	UINT m_ShortcutWidth;
	Handle<Timer> m_Timer;
};

}}}
