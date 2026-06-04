//=============
// WrapPanel.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Panel.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//============
// Wrap-Panel
//============

class WrapPanel: public Panel
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<WrapPanel> Create(Window* Parent) { return Object::Create<WrapPanel>(Parent); }

	// Common
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;

protected:
	// Con-/Destructors
	WrapPanel(Window* Parent): Panel(Parent) {}
};

}}