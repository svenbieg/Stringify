//=============
// ScrollBar.h
//=============

#pragma once


//=======
// Using
//=======

#include "Timing/Clock.h"
#include "UI/Controls/Interactive.h"
#include "UI/Orientation.h"


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
	// Using
	using Clock=Timing::Clock;

	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<ScrollBar> Create(Window* Parent, Orientation Orientation=Orientation::Horizontal)
		{
		return Object::Create<ScrollBar>(Parent, Orientation);
		}
	~ScrollBar();

	// Common
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
	// Con-/Destructors
	ScrollBar(Window* Parent, Orientation Orientation);

	// Common
	ScrollBarButton GetButton(POINT const& Point);
	VOID OnClockTick();
	VOID OnPointerDown(Handle<PointerEventArgs> Args);
	VOID OnPointerLeft();
	VOID OnPointerMoved(Handle<PointerEventArgs> Args);
	VOID OnPointerUp(Handle<PointerEventArgs> Args);
	VOID StartScrolling(INT Step);
	VOID StopScrolling();
	Handle<Clock> m_Clock;
	ScrollBarButton m_Highlight;
	Orientation m_Orientation;
	UINT m_Start;
	POINT m_StartPoint;
	INT m_Step;
};

}}