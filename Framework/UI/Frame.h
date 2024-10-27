//=========
// Frame.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/RenderTarget.h"
#include "UI/Controls/Interactive.h"
#include "Window.h"


//===========
// Namespace
//===========

namespace UI {


//=======
// Frame
//=======

class Frame: public Window
{
public:
	// Using
	using Cursor=Graphics::Cursor;
	using FocusReason=UI::Controls::FocusReason;
	using Interactive=UI::Controls::Interactive;
	using KeyEventType=UI::Input::KeyEventType;
	using KeyEventArgs=UI::Input::KeyEventArgs;
	using POINT=Graphics::POINT;
	using PointerButton=UI::Input::PointerButton;
	using PointerEventArgs=UI::Input::PointerEventArgs;
	using PointerEventType=UI::Input::PointerEventType;
	using RECT=Graphics::RECT;
	using RenderTarget=Graphics::RenderTarget;
	using VirtualKey=UI::Input::VirtualKey;

	// Con-/Destructors
	Frame();

	// Common
	Handle<Interactive> GetFocus()const { return m_Focus; }
	Frame* GetFrame()override { return this; }
	POINT GetFrameOffset()const override { return POINT(0, 0); }
	SIZE GetMinSize(RenderTarget* Target)override;
	Interactive* GetPointerCapture()const { return m_PointerCapture; }
	VOID Invalidate(BOOL Rearrange)override;
	Event<Frame> Invalidated;
	BOOL IsKeyDown(VirtualKey Key);
	Event<Frame, KeyEventType, Handle<KeyEventArgs>> KeyEvent;
	virtual VOID KillFocus();
	Event<Frame> PointerCaptured;
	Event<Frame> PointerReleased;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	virtual VOID SetCursor(Handle<Cursor> Cursor) {}
	VOID SetFocus(Interactive* Focus, FocusReason Reason=FocusReason::None);
	virtual VOID SetPointerCapture(Interactive* Capture) { m_PointerCapture=Capture; }

protected:
	// Common
	BOOL DoKey(KeyEventType Type, Handle<KeyEventArgs> Args);
	VOID DoPointer(PointerEventType Type, Handle<PointerEventArgs> Args);
	VOID RenderWindow(Window* Window, RenderTarget* Target, RECT const& Rect, BOOL Override);
	Interactive* m_PointerCapture;

private:
	// Common
	VOID DoPointer(Interactive* Control, PointerEventType Type, Handle<PointerEventArgs> Args);
	BOOL DoPointer(Window* Window, PointerEventType Type, Handle<PointerEventArgs> Args, Interactive** Focus);
	VOID UpdateKeys(KeyEventType Type, VirtualKey Key);
	VOID UpdateKeys(PointerEventType Type, PointerButton Button);
	Interactive* m_Focus;
	BYTE m_Keys[256];
};

}
