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
	Button(Window* Parent=nullptr, Handle<String> Text=nullptr);

	// Common
	BOOL Border;
	Event<Button> Clicked;
	Handle<Brush> GetBackgroundBrush()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	RECT Padding;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	Handle<String> Text;

private:
	// Common
	VOID DoClick();
	VOID OnFocused();
	VOID OnFocusLost();
	VOID OnInteractiveClicked();
	VOID OnPointerEntered();
	VOID OnPointerLeft();
};

}}