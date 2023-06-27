//================
// ToolBarPanel.h
//================

#pragma once


//=======
// Using
//=======

#include "UI/Controls/StackPanel.h"
#include "ToolBarItem.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//======================
// Forward-Declarations
//======================

class ToolBar;


//================
// Tool-Bar-Panel
//================

class ToolBarPanel: public StackPanel
{
public:
	// Con-/Destructors
	ToolBarPanel(ToolBar* Parent);

	// Common
	Handle<ToolBarItem> Add(Handle<Sentence> ToolTip=nullptr);
	VOID Render(RenderTarget* Target, RECT& Rect)override;
};

}}}