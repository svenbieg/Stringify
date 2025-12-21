//=============
// TextBlock.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Control.h"
#include "DynamicHandle.h"


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
	static inline Handle<TextBlock> Create(Window* Parent, Handle<String> Text=nullptr)
		{
		return new TextBlock(Parent, Text);
		}

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	DynamicHandle<TextBlock, String> Text;

protected:
	// Con-/Destructors
	TextBlock(Window* Parent, Handle<String> Text=nullptr);

private:
	// Common
	VOID OnTextChanged(Handle<String> Text);
};

}}