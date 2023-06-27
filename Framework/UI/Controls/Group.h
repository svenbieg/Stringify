//=========
// Group.h
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
// Group
//=======

class Group: public Control
{
public:
	// Con-/Destructors
	Group(Window* Parent=nullptr, Handle<String> Title=nullptr);

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	RECT Padding;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	Handle<String> Title;
};

}}