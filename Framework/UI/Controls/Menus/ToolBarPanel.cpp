//==================
// ToolBarPanel.cpp
//==================

#include "UI/Controls/Menus/ToolBarPanel.h"


//=======
// Using
//=======

#include "UI/Controls/Menus/ToolBar.h"

using namespace Culture;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//========
// Common
//========

Handle<ToolBarItem> ToolBarPanel::Add(Handle<Sentence> tool_tip)
{
return ToolBarItem::Create(this, tool_tip);
}

VOID ToolBarPanel::Render(RenderTarget* target, RECT& rc)
{
StackPanel::Render(target, rc);
POINT from(rc.Left-4, rc.Top+4);
POINT to(rc.Left-4, rc.Bottom-4);
auto brush=m_Theme->BorderBrush;
target->DrawLine(from, to, brush);
}


//==========================
// Con-/Destructors Private
//==========================

ToolBarPanel::ToolBarPanel(ToolBar* parent):
StackPanel(parent)
{
Padding.Set(8, 0, 0, 0);
}

}}}