//================
// PointerEvent.h
//================

#include "PointerEvent.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Input {


//==================
// Con-/Destructors
//==================

PointerEventArgs::PointerEventArgs(PointerButton button, POINT const& pt, INT delta):
Button(button),
Delta(delta),
Handled(false),
Point(pt)
{}

}}