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
	static inline Handle<ToolBarPanel> Create(ToolBar* Parent) { return new ToolBarPanel(Parent); }

	// Common
	Handle<ToolBarItem> Add(Handle<Culture::Sentence> ToolTip=nullptr);
	VOID Render(RenderTarget* Target, RECT& Rect)override;

private:
	// Con-/Destructors
	ToolBarPanel(ToolBar* Parent);
};

}}}