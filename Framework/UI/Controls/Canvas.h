//==========
// Canvas.h
//==========

#pragma once


//=======
// Using
//=======

#include "Control.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Canvas
//========

class Canvas: public Control
{
public:
	// Con-/Destructors
	Canvas(Window* Parent=nullptr);

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
};

}}