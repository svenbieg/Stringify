//============
// ComboBox.h
//============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Button.h"
#include "UI/Controls/Grid.h"
#include "UI/Controls/TextBox.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//===========
// Combo-Box
//===========

class ComboBox: public Grid
{
public:
	// Con-/Destructors
	static inline Handle<ComboBox> Create(Window* Parent) { return new ComboBox(Parent); }

private:
	// Con-/Destructors
	ComboBox(Window* Parent);

	// Common
	VOID OnButtonClick();
	Handle<Button> m_Button;
	Handle<TextBox> m_TextBox;
};

}}