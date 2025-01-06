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
	~Label();
	static inline Handle<Label> Create(Window* Parent, Handle<Variable> Variable) { return new Label(Parent, Variable); }

private:
	// Con-/Destructors
	Label(Window* Parent, Handle<Variable> Variable);

	// Common
	VOID OnVariableChanged();
	Handle<Variable> m_Variable;
};

}}