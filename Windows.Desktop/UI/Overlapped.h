//==============
// Overlapped.h
//==============

#pragma once


//=======
// Using
//=======

#include "Graphics/Direct2D/RenderTarget.h"
#include "UI/Frame.h"


//===========
// Namespace
//===========

namespace UI {


//============
// Overlapped
//============

class Overlapped: public Frame
{
public:
	// Using
	using D2DRenderTarget=Graphics::Direct2D::RenderTarget;

	// Common
	VOID BringToFront()override;
	RECT GetBorderWidth()const;
	RECT GetClientRect()const override;
	inline HWND GetHandle()const { return m_Handle; }
	SIZE GetMinSize(RenderTarget* Target)override;
	POINT GetScreenOffset()const override;
	inline Handle<RenderTarget> GetTarget()override { return m_RenderTarget; }
	Handle<Theme> GetTheme()override;
	VOID Move(RECT const& Rect)override;
	VOID Repaint();
	VOID SetCursor(Handle<Cursor> Cursor)override;
	VOID SetPointerCapture(Interactive* Capture)override;
	VOID Show(INT Show);
	VOID Shrink();

protected:
	// Con-/Destructors
	Overlapped(Overlapped* Parent=nullptr);

	// Common
	virtual LRESULT HandleMessage(UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled);
	HWND m_Handle;
	Handle<RenderTarget> m_RenderTarget;

private:
	// Common
	VOID OnInvalidated();
	VOID OnVisibleChanged(BOOL Visible);
	static LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
	HCURSOR m_Cursor;
};

}