//===============
// ToolBarItem.h
//===============

#pragma once


//=======
// Using
//=======

#include "Graphics/Icon.h"
#include "UI/Controls/Interactive.h"
#include "ToolTip.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//======================
// Forward-Declarations
//======================

class ToolBarPanel;


//===============
// Tool-Bar-Item
//===============

class ToolBarItem: public Interactive
{
public:
	// Using
	using Bitmap=Graphics::Bitmap;
	using Sentence=Culture::Sentence;

	// Con-/Destructors
	ToolBarItem(ToolBarPanel* Parent, Handle<Sentence> ToolTip=nullptr);

	// Common
	Event<ToolBarItem> Clicked;
	Handle<Brush> GetBackgroundBrush()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	Handle<Graphics::Icon> Icon;
	RECT Padding;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	Handle<Menus::ToolTip> ToolTip;

private:
	// Common
	VOID DoClick();
	VOID OnClicked();
	VOID OnFocused();
	VOID OnFocusLost();
	VOID OnPointerEntered();
	VOID OnPointerLeft();
	Handle<Bitmap> m_Icon;
	Handle<Bitmap> m_IconDisabled;
};

}}}