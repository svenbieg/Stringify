//===========
// TabView.h
//===========

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"
#include "UI/Controls/Control.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==========
// Tab-View
//==========

class TabView: public Control
{
public:
	// Con-/Destructors
	static inline Handle<TabView> Create(Window* Parent) { return new TabView(Parent); }

private:
	// Con-/Destructors
	TabView(Window* Parent);
};

}}