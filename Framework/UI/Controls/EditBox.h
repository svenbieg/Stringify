//===========
// EditBox.h
//===========

#pragma once


//=======
// Using
//=======

#include "Input.h"
#include "ScrollBox.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==========
// Edit-Box
//==========

class EditBox: public ScrollBox
{
public:
	// Con-/Destructors
	static inline Handle<EditBox> Create(Window* Parent) { return new EditBox(Parent); }

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	Property<EditBox, TCHAR> Mask;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	Event<EditBox> SelectionChanged;
	DynamicHandle<EditBox, String> Text;

private:
	// Con-/Destructors
	EditBox(Window* Parent);

	// Common
	VOID OnInputFocused(FocusReason Reason);
	VOID OnInputSelectionChanged();
	VOID OnMaskChanged(TCHAR Mask);
	VOID OnTextChanged(Handle<String> Text);
	Handle<Input> m_Input;
};

}}