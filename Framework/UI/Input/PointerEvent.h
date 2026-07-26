//================
// PointerEvent.h
//================

#pragma once


//=======
// Using
//=======

#include "Graphics/Point.h"
#include "Object.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Input {


//====================
// Pointer-Event-Type
//====================

enum class PointerEventType
{
ButtonDown,
ButtonUp,
Move,
Wheel
};


//================
// Pointer-Button
//================

enum class PointerButton
{
None,
Left,
Right,
Wheel
};


//====================
// Pointer-Event-Args
//====================

class PointerEventArgs: public Object
{
public:
	// Using
	using POINT=Graphics::POINT;
	
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<PointerEventArgs> Create(PointerButton Button, POINT const& Point, INT Delta=0)
		{
		return Object::Create<PointerEventArgs>(Button, Point, Delta);
		}

	// Common
	PointerButton Button;
	INT Delta;
	BOOL Handled;
	POINT Point;

private:
	// Con-/Destructors
	PointerEventArgs(PointerButton Button, POINT const& Point, INT Delta):
		Button(Button),
		Delta(Delta),
		Handled(false),
		Point(Point)
		{}

};

}}