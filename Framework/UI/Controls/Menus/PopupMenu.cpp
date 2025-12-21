//===============
// PopupMenu.cpp
//===============

#include "PopupMenu.h"


//=======
// Using
//=======

#include "UI/Controls/Menus/MenuBar.h"
#include "UI/Application.h"

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

VOID PopupMenu::Close()
{
Menu::Close();
SetParent(nullptr);
m_Popup=nullptr;
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
	auto label=item->Text;
	if(label)
		{
		SIZE size=target->MeasureText(font, scale, label->Begin());
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

VOID PopupMenu::Show(POINT const& pt)
{
Opened(this);
auto app=Application::GetCurrent();
auto current=app->GetCurrentMenu();
if(current)
	{
	if(!IsParentMenu(current))
		current->Close();
	}
app->SetCurrentMenu(this);
BOOL keyboard=false;
if(m_ParentMenu)
	keyboard|=m_ParentMenu->HasKeyboardAccess();
FlagHelper::Set(m_MenuFlags, MenuFlags::KeyboardAccess, keyboard);
RECT rc(pt.Left, pt.Top, pt.Left, pt.Top);
m_Popup=Popup::Create(this);
m_Popup->FocusLost.Add(this, &PopupMenu::OnPopupLostFocus);
m_Popup->Move(rc);
m_Popup->Visible=true;
}


//============================
// Con-/Destructors Protected
//============================

PopupMenu::PopupMenu(Menu* parent):
StackPanel(nullptr, Orientation::Vertical),
Menu(parent)
{
AlignChildren=Alignment::Stretch;
Padding.Set(4, 4, 4, 4);
m_Panel=this;
}


//================
// Common Private
//================

VOID PopupMenu::OnPopupLostFocus()
{
Close();
}

}}}