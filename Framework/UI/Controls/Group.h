//=========
// Group.h
//=========

#pragma once


//=======
// Using
//=======

#include "Culture/Sentence.h"
#include "Control.h"
#include "DynamicHandle.h"


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
	// Using
	using Sentence=Culture::Sentence;

	// Con-/Destructors
	static inline Handle<Group> Create(Window* Parent, Handle<Sentence> Title=nullptr) { return new Group(Parent, Title); }

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	RECT Padding;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	DynamicHandle<Group, Sentence> Title;

private:
	// Con-/Destructors
	Group(Window* Parent, Handle<Sentence> Title);

	// Common
	VOID OnTitleChanged();
};

}}