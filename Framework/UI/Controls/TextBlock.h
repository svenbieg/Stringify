//=============
// TextBlock.h
//=============

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


//============
// Text-Block
//============

class TextBlock: public Control
{
public:
	// Con-/Destructors
	TextBlock(Handle<String> Text=nullptr);
	TextBlock(Window* Parent, Handle<String> Text=nullptr);

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	DynamicHandle<TextBlock, String> Text;
	Handle<Brush> TextColor;

private:
	// Common
	VOID OnTextChanged(Handle<String> Text);
};

}}