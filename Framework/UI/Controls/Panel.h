//=========
// Panel.h
//=========

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


//=======
// Panel
//=======

class Panel: public Control
{
public:
	// Con-/Destructors
	Panel(Window* Parent=nullptr);

	// Common
	BOOL Border;
	virtual Handle<Brush> GetBorderBrush();
	virtual SIZE GetMinSize(RenderTarget* Target)override;
	RECT Padding;
	virtual VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	virtual VOID Render(RenderTarget* Target, RECT& Rect)override;
};

}}