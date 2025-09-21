//==============
// Overlapped.h
//==============

#pragma once


//=======
// Using
//=======

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
	// Con-/Destructors
	~Overlapped();

	// Common
	VOID BringToFront()override;
	Event<Overlapped> FocusLost;
	Handle<Brush> GetBackground()override;
	RECT GetBorderWidth()const;
	RECT GetClientRect()const override;
	inline HWND GetHandle()const { return m_Handle; }
	SIZE GetMinSize(RenderTarget* Target)override;
	RenderTarget* GetRenderTarget()const override;
	POINT GetScreenOffset()const override;
	VOID Move(RECT const& Rect)override;
	VOID Repaint();
	VOID SetCursor(Cursor* Cursor)override;
	VOID SetPointerCapture(Interactive* Capture)override;
	VOID Show(INT Show);
	VOID Shrink();

protected:
	// Con-/Destructors
	Overlapped();

	// Common
	virtual LRESULT HandleMessage(UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled);
	HWND m_Handle;
	Handle<RenderTarget> m_RenderTarget;
	Handle<Theme> m_Theme;

private:
	// Common
	VOID OnInvalidated();
	VOID OnThemeChanged();
	VOID OnVisibleChanged(BOOL Visible);
	static LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
	HCURSOR m_Cursor;
};

}