//=============
// ScrollBar.h
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
	~ScrollBar();

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
	VOID OnSystemTimer();
	VOID StartScrolling(INT Step);
	VOID StopScrolling();
	ScrollBarButton m_Highlight;
	Orientation m_Orientation;
	UINT m_Start;
	POINT m_StartPoint;
	INT m_Step;
};

}}
