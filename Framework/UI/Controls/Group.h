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
	static inline Handle<Group> Create(Window* Parent, Handle<String> Title=nullptr) { return new Group(Parent, Title); }

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	RECT Padding;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	Handle<String> Title;

private:
	// Con-/Destructors
	Group(Window* Parent, Handle<String> Title);
};

}}