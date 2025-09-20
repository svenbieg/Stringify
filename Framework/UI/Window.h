//==========
// Window.h
//==========

#pragma once


//=======
// Using
//=======

#include "Collections/List.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Theme.h"
#include "FlagHelper.h"
#include "Property.h"


//===========
// Namespace
//===========

namespace UI {


//======================
// Forward-Declarations
//======================

class Frame;


//========
// Window
//========

class Window: public Object
{
public:
	// Using
	using Brush=Graphics::Brush;
	using ChildList=Collections::List<Handle<Window>>;
	using POINT=Graphics::POINT;
	using RECT=Graphics::RECT;
	using RenderTarget=Graphics::RenderTarget;
	using SIZE=Graphics::SIZE;
	using Theme=Graphics::Theme;

	// Common
	virtual VOID BringToFront();
	Handle<ChildList> Children;
	virtual Handle<Brush> GetBackground();
	virtual RECT GetClientRect()const;
	inline Frame* GetFrame() { return m_Frame; }
	virtual POINT GetFrameOffset()const;
	RECT GetFrameRect()const;
	virtual SIZE GetMinSize(RenderTarget* Target);
	POINT GetOffset()const { return POINT(m_Rect.Left, m_Rect.Top); }
	inline Window* GetParent()const { return m_Parent; }
	inline RECT const& GetRect()const { return m_Rect; }
	inline RenderTarget* GetRenderTarget()const { return m_RenderTarget; }
	FLOAT GetScaleFactor()const;
	virtual POINT GetScreenOffset()const;
	RECT GetScreenRect()const;
	inline Theme* GetTheme()const { return m_Theme; }
	Handle<Window> GetVisibleChild(UINT Id);
	virtual VOID Invalidate(BOOL Rearrange=false);
	BOOL IsInvalidated() { return FlagHelper::Get(m_Flags, WindowFlags::Repaint); }
	BOOL IsParentOf(Window* Child);
	BOOL IsVisible();
	SIZE MinSize;
	virtual VOID Move(RECT const& Rect);
	VOID Move(RenderTarget* Target, RECT const& Rect);
	virtual VOID Rearrange(RenderTarget* Target, RECT& Rect) {}
	virtual VOID Render(RenderTarget* Target, RECT& Rect);
	Event<Window, RenderTarget*, RECT&> Rendered;
	FLOAT Scale;
	VOID SetPosition(POINT const& Position);
	VOID Validate() { FlagHelper::Clear(m_Flags, WindowFlags::Repaint); }
	Property<Window, BOOL> Visible;

protected:
	// Flags
	enum class WindowFlags: UINT
		{
		None=0,
		Rearrange=1,
		Repaint=2,
		Update=3
		};

	// Con-/Destructors
	Window(Window* Parent);

	// Common
	WindowFlags m_Flags;
	Frame* m_Frame;
	Window* m_Parent;
	RECT m_Rect;
	RenderTarget* m_RenderTarget;
	Theme* m_Theme;

private:
	// Common
	VOID OnVisibleChanged(BOOL Visible);
};

}