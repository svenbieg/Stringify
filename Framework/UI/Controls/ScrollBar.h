//=============
// ScrollBar.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Orientation.h"
#include "UI/Timer.h"
#include "Interactive.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//=========
// Buttons
//=========

enum class ScrollBarButton
{
None,
First,
Second,
Bar
};


//============
// Visibility
//============

enum class ScrollBarVisibility
{
Auto,
Hidden,
Visible
};


//============
// Scroll-Bar
//============

class ScrollBar: public Interactive
{
public:
	// Con-/Destructors
	ScrollBar(Window* Parent, Orientation Orientation=Orientation::Horizontal);

	// Common
	Handle<Brush> GetBackgroundBrush()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	UINT GetWidth();
	FLOAT Fraction;
	UINT Position;
	UINT Range;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	Event<ScrollBar> Scrolled;
	INT Step;
	ScrollBarVisibility Visibility;
	UINT Width;

private:
	// Common
	ScrollBarButton GetButton(POINT const& Point);
	VOID OnPointerDown(Handle<PointerEventArgs> Args);
	VOID OnPointerLeft();
	VOID OnPointerMoved(Handle<PointerEventArgs> Args);
	VOID OnPointerUp(Handle<PointerEventArgs> Args);
	VOID OnScrollTimer();
	VOID StartScrolling(INT Step);
	VOID StopScrolling();
	Handle<Timer> hScrollTimer;
	INT iStep;
	POINT m_StartPoint;
	ScrollBarButton uHighlight;
	Orientation uOrientation;
	UINT uStart;
};

}}
