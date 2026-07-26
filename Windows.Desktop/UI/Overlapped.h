//==============
// Overlapped.h
//==============

#pragma once


//=======
// Using
//=======

#include "Culture/Sentence.h"
#include "Graphics/Icon.h"
#include "UI/Frame.h"
#include "DynamicHandle.h"


//===========
// Namespace
//===========

namespace UI {


//==============
// Minimization
//==============

enum class Minimization: UINT
{
Size
};


//============
// Overlapped
//============

class Overlapped: public Frame
{
public:
	// Using
	using Sentence=Culture::Sentence;

	// Common
	VOID Activate(FocusReason Reason)override;
	VOID BringToFront()override;
	VOID Close();
	Handle<Brush> GetBackground()override;
	RECT GetBorderWidth()const;
	RECT GetClientRect()const override;
	inline HWND GetHandle()const { return m_Handle; }
	SIZE GetMinSize(RenderTarget* Target)override;
	RenderTarget* GetRenderTarget()const override;
	POINT GetScreenOffset()const override;
	DynamicHandle<Overlapped, Graphics::Icon> Icon;
	VOID Minimize();
	VOID Minimize(Minimization);
	VOID Move(RECT const& Rect)override;
	VOID Repaint();
	VOID SetCursor(Cursor* Cursor)override;
	VOID SetPointerCapture(Interactive* Capture)override;
	VOID Show(INT Show);
	DynamicHandle<Overlapped, Sentence> Title;

protected:
	// Con-/Destructors
	Overlapped(Overlapped* Parent=nullptr);

	// Common
	virtual LRESULT HandleMessage(UINT Message, WPARAM WParam, LPARAM LParam);
	virtual UINT Release()noexcept override;
	HCURSOR m_Cursor;
	HWND m_Handle;
	HICON m_IconBig;
	HICON m_IconSmall;
	Overlapped* m_Parent;
	Handle<RenderTarget> m_RenderTarget;

private:
	// Common
	VOID OnIconChanged(Handle<Graphics::Icon> Icon);
	VOID OnInvalidated();
	VOID OnThemeChanged();
	VOID OnTitleChanged(Handle<Sentence> Title);
	VOID OnVisibleChanged(BOOL Visible);
	static LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
};

}