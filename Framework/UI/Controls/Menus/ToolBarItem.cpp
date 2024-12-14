//=================
// ToolBarItem.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Concurrency/MainTask.h"
#include "MenuHelper.h"
#include "ToolBarItem.h"
#include "ToolBarPanel.h"

using namespace Concurrency;
using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

ToolBarItem::ToolBarItem(ToolBarPanel* parent, Handle<Sentence> tool_tip):
Interactive(parent),
Padding(3, 3, 3, 3)
{
Interactive::Clicked.Add(this, &ToolBarItem::OnClicked);
Focused.Add(this, &ToolBarItem::OnFocused);
FocusLost.Add(this, &ToolBarItem::OnFocusLost);
PointerEntered.Add(this, &ToolBarItem::OnPointerEntered);
PointerLeft.Add(this, &ToolBarItem::OnPointerLeft);
if(tool_tip)
	ToolTip=new Menus::ToolTip(this, tool_tip);
}


//========
// Common
//========

Handle<Brush> ToolBarItem::GetBackgroundBrush()
{
auto theme=GetTheme();
auto background=theme->ControlBrush;
BOOL focus=HasFocus();
focus|=HasPointerFocus();
if(!Enabled)
	focus=false;
if(focus)
	background=theme->HighlightBrush;
return background;
}

Graphics::SIZE ToolBarItem::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
SIZE size(16, 16);
size*=scale;
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID ToolBarItem::Render(RenderTarget* target, RECT& rc)
{
Interactive::Render(target, rc);
if(!Icon)
	return;
auto theme=GetTheme();
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
if(!m_Icon)
	m_Icon=Icon->GetBitmap(16*scale);
auto icon=m_Icon;
if(!Enabled)
	{
	if(!m_IconDisabled)
		{
		m_IconDisabled=m_Icon->Copy();
		DisableMenuBitmap(m_IconDisabled);
		}
	icon=m_IconDisabled;
	}
SIZE ico_size=icon->GetDimensions();
RECT ico_rc(ico_size);
target->DrawBitmap(rc, icon, ico_rc);
}


//================
// Common Private
//================

VOID ToolBarItem::DoClick()
{
Clicked(this);
}

VOID ToolBarItem::OnClicked()
{
MainTask::Dispatch(this, &ToolBarItem::DoClick);
}

VOID ToolBarItem::OnFocused()
{
Invalidate();
}

VOID ToolBarItem::OnFocusLost()
{
Invalidate();
}

VOID ToolBarItem::OnPointerEntered()
{
Invalidate();
}

VOID ToolBarItem::OnPointerLeft()
{
Invalidate();
}

}}}