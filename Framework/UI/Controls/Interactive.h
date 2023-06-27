//===============
// Interactive.h
//===============

#pragma once


//=======
// Using
//=======

#include "Graphics/Cursor.h"
#include "UI/Input/KeyEvent.h"
#include "UI/Input/PointerEvent.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==============
// Focus-Reason
//==============

enum class FocusReason
{
None,
Keyboard,
Pointer,
Software
};


//=============
// Interactive
//=============

class Interactive: public Control
{
public:
	// Using
	using Cursor=Graphics::Cursor;
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using PointerButton=UI::Input::PointerButton;
	using PointerEventArgs=UI::Input::PointerEventArgs;
	using VirtualKey=UI::Input::VirtualKey;

	// Con-/Destructors
	Interactive(Window* Parent=nullptr);

	// Common
	VOID CapturePointer();
	Event<Interactive, Handle<PointerEventArgs>> Clicked;
	Property<Interactive, BOOL> Enabled;
	Event<Interactive, FocusReason> Focused;
	Event<Interactive> FocusLost;
	virtual Handle<Cursor> GetCursor() { return nullptr; }
	BOOL HasFocus();
	BOOL HasPointerFocus();
	static Interactive* GetNextControl(Window* Window, Interactive* Control, INT Relative);
	BOOL IsCapturingPointer();
	BOOL IsEnabled();
	Event<Interactive, Handle<KeyEventArgs>> KeyDown;
	Event<Interactive, Handle<KeyEventArgs>> KeyUp;
	BOOL KillFocus();
	Event<Interactive, Handle<PointerEventArgs>> PointerDown;
	Event<Window> PointerEntered;
	Event<Window> PointerLeft;
	Event<Interactive, Handle<PointerEventArgs>> PointerMoved;
	Event<Interactive, Handle<PointerEventArgs>> PointerUp;
	Event<Interactive, Handle<PointerEventArgs>> PointerWheel;
	VOID ReleasePointer();
	VOID SetFocus(FocusReason Reason=FocusReason::None);
	BOOL TabStop;

private:
	// Flags
	enum class InteractiveFlags
		{
		None=0,
		LeftButtonDown=1
		};

	// Common
	static BOOL GetNext(Window* Window, Interactive* Control, Interactive** Next, INT* Relative, BOOL Forward, UINT Level);
	VOID OnEnabledChanged();
	VOID OnFocused();
	VOID OnFocusLost();
	VOID OnKeyDown(Handle<KeyEventArgs> Args);
	VOID OnPointerDown(Handle<PointerEventArgs> Args);
	VOID OnPointerEntered();
	VOID OnPointerLeft();
	VOID OnPointerUp(Handle<PointerEventArgs> Args);
	InteractiveFlags uInteractiveFlags;
};

}}