//=============
// ToolBar.cpp
//=============

#include "UI/Controls/Menus/ToolBar.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//========
// Common
//========

Handle<ToolBarPanel> ToolBar::AddPanel()
{
return ToolBarPanel::Create(this);
}


//==========================
// Con-/Destructors Private
//==========================

ToolBar::ToolBar(Window* parent):
WrapPanel(parent)
{
Padding.Set(-6, 0, 0, 0);
}

}}}