//===========
// ToolBar.h
//===========

#pragma once


//=======
// Using
//=======

#include "UI/Controls/WrapPanel.h"
#include "ToolBarPanel.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==========
// Tool-Bar
//==========

class ToolBar: public WrapPanel
{
public:
	// Con-/Destructors
	static inline Handle<ToolBar> Create(Window* Parent) { return new ToolBar(Parent); }

	// Common
	Handle<ToolBarPanel> AddPanel();

private:
	// Con-/Destructors
	ToolBar(Window* Parent);
};

}}}