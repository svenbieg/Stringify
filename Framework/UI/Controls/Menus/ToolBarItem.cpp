//=================
// ToolBarItem.cpp
//=================

#include "ToolBarItem.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "UI/Controls/Menus/MenuHelper.h"
#include "UI/Controls/Menus/ToolBarPanel.h"

using namespace Concurrency;
using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//========
// Common
//========

Handle<Brush> ToolBarItem::GetBackground()
{
if(!m_Theme)
	return nullptr;
auto brush=m_Theme->ControlBrush;
BOOL has_focus=HasFocus();
has_focus|=HasPointerFocus();
if(has_focus)
	brush=m_Theme->HighlightBrush;
return brush;
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
		MenuHelper::GetBitmapDisabled(m_IconDisabled);
		}
	icon=m_IconDisabled;
	}
SIZE ico_size=icon->GetDimensions();
RECT ico_rc(ico_size);
target->DrawBitmap(rc, icon, ico_rc);
}


//==========================
// Con-/Destructors Private
//==========================

ToolBarItem::ToolBarItem(ToolBarPanel* parent, Handle<Sentence> tool_tip):
Interactive(parent),
Padding(3, 3, 3, 3)
{
Interactive::Clicked.Add(this, &ToolBarItem::OnClicked);
TabStop=true;
if(tool_tip)
	ToolTip=Menus::ToolTip::Create(this, tool_tip);
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
DispatchedQueue::Append(this, &ToolBarItem::DoClick);
}

}}}