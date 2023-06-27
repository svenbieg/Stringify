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
	WrapPanel(Window* Parent=nullptr);

	// Common
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
};

}}