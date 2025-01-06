//===========
// Label.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Label.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Label::~Label()
{
if(m_Variable)
	{
	m_Variable->Changed.Remove(this);
	m_Variable=nullptr;
	}
}


//==========================
// Con-/Destructors Private
//==========================

Label::Label(Window* parent, Handle<Variable> var):
TextBlock(parent),
m_Variable(var)
{
Text=m_Variable->ToString();
m_Variable->Changed.Add(this, &Label::OnVariableChanged);
}


//================
// Common Private
//================

VOID Label::OnVariableChanged()
{
Text=m_Variable->ToString();
}

}}