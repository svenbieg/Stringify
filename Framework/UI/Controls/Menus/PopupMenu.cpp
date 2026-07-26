//===============
// PopupMenu.cpp
//===============

#include "PopupMenu.h"


//=======
// Using
//=======

#include "UI/Controls/Menus/MenuBar.h"

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

Handle<PopupMenuItem> PopupMenu::Add(Handle<Sentence> label)
{
return PopupMenuItem::Create(this, label);
}

VOID PopupMenu::Close(FocusReason reason)
{
if(!m_Popup)
	return;
Menu::Close(reason);
m_Popup=nullptr;
Visible.Set(false, EventNotification::None);
}

Graphics::SIZE PopupMenu::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
auto font=m_Theme->DefaultFont;
UINT label_width=0;
UINT shortcut_width=0;
bool icon=false;
bool arrow=false;
bool separator=true;
Handle<UI::Window> last_sep;
for(auto it=m_Panel->Children->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	auto item=child.As<PopupMenuItem>();
	if(!item)
		continue;
	if(item->IsSeparator())
		{
		item->Visible=!separator;
		if(item->Visible)
			last_sep=item;
		separator=true;
		continue;
		}
	if(!item->Visible)
		continue;
	separator=false;
	last_sep=nullptr;
	if(item->Icon)
		icon=true;
	auto text=item->Text;
	if(text)
		{
		SIZE size=target->MeasureText(font, scale, text->Begin());
		label_width=TypeHelper::Max(label_width, size.Width);
		}
	auto shortcut=item->Shortcut;
	if(shortcut)
		{
		SIZE size=target->MeasureText(font, scale, shortcut->Begin());
		shortcut_width=TypeHelper::Max(shortcut_width, size.Width);
		}
	auto sub_menu=item->SubMenu;
	if(sub_menu)
		arrow=true;
	}
if(last_sep)
	last_sep->Visible=false;
UINT icon_width=0;
if(icon)
	{
	icon_width=font->GetSize()*scale*0.9f;
	icon_width+=8*scale;
	}
label_width+=12*scale;
if(arrow)
	{
	SIZE arrow_size=target->MeasureText(font, scale, TEXT(">"));
	shortcut_width+=arrow_size+10*scale;
	}
for(auto it=m_Panel->Children->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	auto item=child.As<PopupMenuItem>();
	if(!item)
		continue;
	item->SetColumns(icon_width, label_width, shortcut_width);
	}
return StackPanel::GetMinSize(target);
}

VOID PopupMenu::Show(POINT const& pt, FocusReason reason)
{
s_Current=this;
if(!m_Popup)
	{
	Visible=true;
	Opened(this);
	RECT rc(pt.Left, pt.Top, pt.Left, pt.Top);
	Popup* parent=nullptr;
	auto popup_menu=dynamic_cast<PopupMenu*>(m_ParentMenu);
	if(popup_menu)
		parent=popup_menu->GetPopup();
	m_Popup=Popup::Create(parent);
	m_Popup->Content=this;
	m_Popup->FocusLost.Add(this, &PopupMenu::OnPopupFocusLost);
	m_Popup->Move(rc);
	m_Popup->Visible=true;
	}
if(reason==FocusReason::Keyboard)
	{
	Select(reason);
	if(m_ParentMenu)
		m_ParentMenu->Invalidate();
	}
}


//============================
// Con-/Destructors Protected
//============================

PopupMenu::PopupMenu(Menu* parent):
StackPanel(nullptr, Orientation::Vertical),
Menu(parent, this)
{
AlignChildren=Alignment::Stretch;
Padding.Set(4, 4, 4, 4);
Visible.Set(false, EventNotification::None);
}


//================
// Common Private
//================

VOID PopupMenu::OnPopupFocusLost(Frame* sender, FocusReason reason, Frame* active)
{
if(s_Current!=this)
	return;
auto popup=dynamic_cast<Popup*>(active);
if(popup)
	return;
Exit(reason);
}

}}}