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

	// Con-/Destructors
	PointerEventArgs(PointerButton Button, POINT const& Point, INT Delta=0);

	// Common
	PointerButton Button;
	INT Delta;
	BOOL Handled;
	POINT Point;
};

}}