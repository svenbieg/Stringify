//============
// TreeView.h
//============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/ScrollBox.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//===========
// Tree-View
//===========

class TreeView: public ScrollBox
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<TreeView> Create(Window* Parent)
		{
		return Object::Create<TreeView>(Parent);
		}

protected:
	// Con-/Destructors
	TreeView(Window* Parent);
};

}}