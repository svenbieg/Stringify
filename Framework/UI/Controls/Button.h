//==========
// Button.h
//==========

#pragma once


//=======
// Using
//=======

#include "Interactive.h"
#include "TextBlock.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Button
//========

class Button: public Interactive
{
public:
	// Con-/Destructors
	static inline Handle<Button> Create(Window* Parent, Handle<String> Text=nullptr)
		{
		return new Button(Parent, Text);
		}

	// Common
	BOOL Border;
	Event<Button> Clicked;
	SIZE GetMinSize(RenderTarget* Target)override;
	RECT Padding;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	Handle<String> Text;

private:
	// Con-/Destructors
	Button(Window* Parent, Handle<String> Text);

	// Common
	VOID DoClick();
	VOID OnFocused();
	VOID OnFocusLost();
	VOID OnInteractiveClicked();
	VOID OnPointerEntered();
	VOID OnPointerLeft();
};

}}