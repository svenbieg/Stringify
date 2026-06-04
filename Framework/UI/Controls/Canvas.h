//==========
// Canvas.h
//==========

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Control.h"


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
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<Canvas> Create(Window* Parent) { return Object::Create<Canvas>(Parent); }

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;

private:
	// Con-/Destructors
	Canvas(Window* Parent): Control(Parent) {}
};

}}