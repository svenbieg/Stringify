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
	VOID BringToFront();
	Handle<ChildList> Children;
	Handle<Graphics::Font> Font;
	virtual Handle<Brush> GetBackgroundBrush();
	RECT GetClientRect()const;
	virtual Handle<Graphics::Font> GetFont();
	virtual Frame* GetFrame();
	POINT GetFrameOffset()const;
	RECT GetFrameRect()const;
	virtual SIZE GetMinSize(RenderTarget* Target);
	POINT GetOffset()const;
	inline RECT const& GetRect()const { return rcRect; }
	FLOAT GetScaleFactor()const;
	virtual Handle<RenderTarget> GetTarget();
	virtual Handle<Theme> GetTheme();
	Handle<Window> GetVisibleChild(UINT Id);
	virtual VOID Invalidate(BOOL Rearrange=false);
	BOOL IsInvalidated() { return GetFlag(uFlags, WindowFlags::Repaint); }
	BOOL IsParentOf(Window* Child);
	BOOL IsVisible();
	SIZE MinSize;
	VOID Move(RECT const& Rect);
	VOID Move(RenderTarget* Target, RECT const& Rect);
	DynamicPointer<Window, Window> Parent;
	virtual VOID Rearrange(RenderTarget* Target, RECT& Rect) {}
	virtual VOID Render(RenderTarget* Target, RECT& Rect);
	Event<Window, RenderTarget*, RECT&> Rendered;
	FLOAT Scale;
	VOID SetPosition(POINT const& Position);
	VOID Validate() { ClearFlag(uFlags, WindowFlags::Repaint); }
	Property<Window, BOOL> Visible;

protected:
	// Flags
	enum class WindowFlags: UINT
		{
		None=0,
		Rearrange=1,
		Repaint=2
		};

	// Con-/Destructors
	Window(Window* Parent=nullptr);

	// Common
	RECT rcRect;
	WindowFlags uFlags;

private:
	// Common
	VOID OnParentChanged(Window* Parent);
	VOID OnVisibleChanged(BOOL Visible);
	Window* pOldParent;
};

}