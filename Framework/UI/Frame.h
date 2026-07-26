//=========
// Frame.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/RenderTarget.h"
#include "UI/Controls/Interactive.h"
#include "UI/Window.h"


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

	// Friends
	friend Interactive;
	friend Object;

	// Con-/Destructors
	~Frame();
	static inline Handle<Frame> Create() { return Object::Create<Frame>(); }

	// Common
	virtual VOID Activate(FocusReason Reason);
	Event<Frame, FocusReason, Frame*> Activated;
	Event<Frame, FocusReason, Frame*> FocusLost;
	VOID FocusNext(FocusReason Reason=FocusReason::None, BOOL Forward=true);
	inline Handle<Interactive> GetFocus()const { return m_Focus; }
	POINT GetFrameOffset()const override { return POINT(0, 0); }
	SIZE GetMinSize(RenderTarget* Target)override;
	Interactive* GetPointerCapture()const { return m_PointerCapture; }
	VOID Invalidate(BOOL Rearrange)override;
	Event<Frame> Invalidated;
	BOOL IsKeyDown(VirtualKey Key);
	Event<Frame, KeyEventType, Handle<KeyEventArgs>> KeyEvent;
	Event<Frame> PointerCaptured;
	Event<Frame> PointerReleased;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	virtual VOID SetCursor(Cursor* Cursor) {}
	VOID SetFocus(Interactive* Focus, FocusReason Reason=FocusReason::None);
	virtual VOID SetPointerCapture(Interactive* Capture);

protected:
	// Con-/Destructors
	Frame();

	// Common
	BOOL DoKey(KeyEventType Type, Handle<KeyEventArgs> Args);
	BOOL DoPointer(PointerEventType Type, Handle<PointerEventArgs> Args);
	VOID RenderWindow(Window* Window, RenderTarget* Target, RECT const& Rect, BOOL Override);
	Interactive* m_Focus;
	BYTE m_KeyDown[Input::VIRTUAL_KEY_COUNT/8];
	Interactive* m_PointerCapture;

private:
	// Common
	BOOL DoPointer(Interactive* Control, PointerEventType Type, Handle<PointerEventArgs> Args);
	BOOL DoPointer(Window* Window, PointerEventType Type, Handle<PointerEventArgs> Args, Interactive** Focus);
	VOID UpdateKeys(KeyEventType Type, VirtualKey Key);
	VOID UpdateKeys(PointerEventType Type, PointerButton Button);
	static Frame* s_Current;
};

}