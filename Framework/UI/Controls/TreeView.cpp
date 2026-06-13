//==============
// TreeView.cpp
//==============

#include "TreeView.h"


//=======
// Using
//=======

#include "UI/Controls/Panel.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//============================
// Con-/Destructors Protected
//============================

TreeView::TreeView(Window* parent):
ScrollBox(parent)
{
Panel::Create(Body);
}

}}