//=============
// WrapPanel.h
//=============

#pragma once


//=======
// Using
//=======

#include "Panel.h"


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
	// Con-/Destructors
	static inline Handle<WrapPanel> Create(Window* Parent) { return new WrapPanel(Parent); }

	// Common
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;

protected:
	// Con-/Destructors
	WrapPanel(Window* Parent): Panel(Parent) {}
};

}}