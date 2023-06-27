//=========
// Frame.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/Direct2D/RenderTarget.h"
#include "UI/Frame.h"
#include "Window.h"


//===========
// Namespace
//===========

namespace Desktop {


//=======
// Frame
//=======

class Frame: public Window
{
public:
	// Using
	using Cursor=Graphics::Cursor;
	using RenderTarget=Graphics::Direct2D::RenderTarget;

	// Con-/Destructors
	Frame(Window* Parent);

	// Common
	Handle<UI::Frame> UIFrame;

protected:
	// Common
	Handle<RenderTarget> hRenderTarget;

private:
	// Flags
	enum class FrameFlags: UINT
		{
		None=0,
		Rearrange=1,
		Repaint=2
		};

	// Common
	VOID DoRepaint();
	VOID OnCreated();
	VOID OnFrameCursorChanged(Handle<Cursor> Cursor);
	VOID OnFrameInvalidated();
	VOID OnFramePointerCaptured();
	VOID OnFramePointerReleased();
	VOID OnMessage(WNDMSG& Message);
	VOID UpdateDpi();
	FrameFlags uFlags;
};

}