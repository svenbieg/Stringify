//=============
// ToolBar.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "ToolBar.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

ToolBar::ToolBar(Window* parent):
WrapPanel(parent)
{
Padding.Set(-6, 0, 0, 0);
}


//========
// Common
//========

Handle<ToolBarPanel> ToolBar::AddPanel()
{
return new ToolBarPanel(this);
}

}}}