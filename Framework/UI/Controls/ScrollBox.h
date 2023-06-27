//=============
// ScrollBox.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Alignment.h"
#include "ScrollBar.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//============
// Scroll-Box
//============

class ScrollBox: public Control
{
public:
	// Using
	using PointerEventArgs=UI::Input::PointerEventArgs;

	// Con-/Destructors
	ScrollBox(Window* Parent=nullptr);

	// Common
	HorizontalAlignment AlignHorizontal;
	VerticalAlignment AlignVertical;
	Handle<Interactive> Body;
	Property<ScrollBox, BOOL> Enabled;
	SIZE GetMinSize(RenderTarget* Target)override;
	Handle<ScrollBar> HorizontalBar;
	virtual VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	VOID Scroll(INT X, INT Y);
	VOID SetHotspot(RECT Rect);
	VOID SetPosition(INT Left, INT Top);
	Handle<ScrollBar> VerticalBar;
	VOID Zoom(FLOAT Factor);
	FLOAT ZoomMax;
	FLOAT ZoomMin;

private:
	// Common
	VOID OnBodyPointerDown(Handle<PointerEventArgs> Args);
	VOID OnBodyPointerMoved(Handle<PointerEventArgs> Args);
	VOID OnBodyPointerUp(Handle<PointerEventArgs> Args);
	VOID OnBodyPointerWheel(Handle<PointerEventArgs> Args);
	VOID OnScrollBarScrolled();
	VOID SetHotspot(RECT& ContentRect, RECT const& BodyRect);
	VOID UpdateBodyRect(RECT& BodyRect, SIZE const& ContentSize, BOOL* ScrollHorizontal, BOOL* ScrollVertical);
	VOID UpdateContentRect(RECT& ContentRect, SIZE const& BodySize);
	VOID ZoomStep(INT Step);
	FLOAT fZoom;
	POINT ptStart;
	POINT ptStartPosition;
	RECT rcHotspot;
};

}}