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
	ToolBar(Window* Parent);

	// Common
	Handle<ToolBarPanel> AddPanel();
};

}}}