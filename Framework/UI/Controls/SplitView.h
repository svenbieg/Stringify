//=============
// SplitView.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Orientation.h"
#include "Interactive.h"


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
	SplitView(Orientation Orientation=Orientation::Vertical);
	SplitView(Window* Parent, Orientation Orientation=Orientation::Vertical);

	// Common
	UINT Distance;
	Handle<Cursor> GetCursor()override;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	UINT Size;

private:
	// Common
	VOID OnPointerDown(Handle<PointerEventArgs> Args);
	VOID OnPointerMoved(Handle<PointerEventArgs> Args);
	VOID OnPointerUp(Handle<PointerEventArgs> Args);
	UINT uStartSize;
	POINT ptStart;
	Orientation uOrientation;
};

}}