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

Label::Label(Handle<Variable> var):
Label(nullptr, var)
{}

Label::Label(Window* parent, Handle<Variable> var):
TextBlock(parent),
m_Variable(var)
{
Text=m_Variable->ToString();
m_Variable->Changed.Add(this, &Label::OnVariableChanged);
}

Label::~Label()
{
m_Variable->Changed.Remove(this);
}


//================
// Common Private
//================

VOID Label::OnVariableChanged()
{
Text=m_Variable->ToString();
}

}}