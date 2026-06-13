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
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<SplitView> Create(Window* Parent, Orientation Orientation=Orientation::Horizontal)
		{
		return Object::Create<SplitView>(Parent, Orientation);
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
	UINT m_Delta;
	POINT m_StartPoint;
	Orientation m_Orientation;
};

}}