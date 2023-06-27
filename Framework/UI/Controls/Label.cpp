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
hVariable(var)
{
Text=hVariable->ToString();
hVariable->Changed.Add(this, &Label::OnVariableChanged);
}

Label::~Label()
{
hVariable->Changed.Remove(this);
}


//================
// Common Private
//================

VOID Label::OnVariableChanged()
{
Text=hVariable->ToString();
}

}}