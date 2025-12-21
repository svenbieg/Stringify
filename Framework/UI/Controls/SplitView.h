//=============
// SplitView.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Interactive.h"
#include "UI/Orientation.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//===========
// SplitView
//===========

class SplitView: public Interactive
{
public:
	// Con-/Destructors
	static inline Handle<SplitView> Create(Window* Parent, Orientation Orientation=Orientation::Vertical)
		{
		return new SplitView(Parent, Orientation);
		}

	// Common
	UINT Distance;
	Handle<Cursor> GetCursor()override;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	UINT Size;

private:
	// Con-/Destructors
	SplitView(Window* Parent, Orientation Orientation);

	// Common
	VOID OnPointerDown(Handle<PointerEventArgs> Args);
	VOID OnPointerMoved(Handle<PointerEventArgs> Args);
	VOID OnPointerUp(Handle<PointerEventArgs> Args);
	POINT m_StartPoint;
	UINT m_StartSize;
	Orientation m_Orientation;
};

}}