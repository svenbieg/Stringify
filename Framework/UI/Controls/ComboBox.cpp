//==============
// ComboBox.cpp
//==============

#include "UI/Controls/ComboBox.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==========================
// Con-/Destructors Private
//==========================

ComboBox::ComboBox(Window* parent):
Grid(parent)
{
AddColumn(0, GridUnit::Star, HorizontalAlignment::Stretch);
AddColumn(0, GridUnit::Auto);
m_TextBox=TextBox::Create(this);
m_Button=Button::Create(this);
m_Button->Text="▼";
}


//================
// Common Private
//================

VOID ComboBox::OnButtonClick()
{
}

}}