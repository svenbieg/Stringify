//=============
// Control.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Control.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//============================
// Con-/Destructors Protected
//============================

Control::Control(Window* parent):
Window(parent),
Margin(0, 0, 0, 0)
{}

}}