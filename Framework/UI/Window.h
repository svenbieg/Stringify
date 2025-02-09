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
	Handle<Brush> Background;
	virtual VOID BringToFront();
	Handle<ChildList> Children;
	Handle<Graphics::Font> Font;
	virtual Handle<Brush> GetBackgroundBrush();
	virtual RECT GetClientRect()const;
	virtual Handle<Graphics::Font> GetFont();
	virtual inline Frame* GetFrame() { return m_Parent->GetFrame(); }
	virtual POINT GetFrameOffset()const;
	RECT GetFrameRect()const;
	virtual SIZE GetMinSize(RenderTarget* Target);
	POINT GetOffset()const { return POINT(m_Rect.Left, m_Rect.Top); }
	inline Window* GetParent()const { return m_Parent; }
	inline RECT const& GetRect()const { return m_Rect; }
	FLOAT GetScaleFactor()const;
	virtual POINT GetScreenOffset()const;
	RECT GetScreenRect()const;
	virtual Handle<RenderTarget> GetTarget();
	virtual Handle<Theme> GetTheme();
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
	Window(Window* Parent=nullptr);

	// Common
	WindowFlags m_Flags;
	Window* m_Parent;
	RECT m_Rect;

private:
	// Common
	VOID OnVisibleChanged(BOOL Visible);
};

}