//=========
// Label.h
//=========

#pragma once


//=======
// Using
//=======

#include "TextBlock.h"
#include "Variable.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//=======
// Label
//=======

class Label: public TextBlock
{
public:
	// Con-/Destructors
	Label(Handle<Variable> Variable);
	Label(Window* Parent, Handle<Variable> Variable);
	~Label();

private:
	// Common
	VOID OnVariableChanged();
	Handle<Variable> m_Variable;
};

}}