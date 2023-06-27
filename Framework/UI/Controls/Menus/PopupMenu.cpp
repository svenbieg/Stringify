//===============
// PopupMenu.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "UI/Frame.h"
#include "MenuBar.h"
#include "PopupMenu.h"

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

PopupMenu::PopupMenu(UI::Frame* frame, Menu* parent):
StackPanel(frame, Orientation::Vertical),
Menu(this, parent)
{
AlignChildren=Alignment::Stretch;
Padding.Set(4, 4, 4, 4);
Visible=false;
}


//========
// Common
//========

Handle<PopupMenuItem> PopupMenu::Add(Handle<Sentence> label)
{
return new PopupMenuItem(this, label);
}

VOID PopupMenu::Close()
{
Menu::Close();
Visible=false;
}

Handle<Brush> PopupMenu::GetBackgroundBrush()
{
auto theme=GetTheme();
return theme->GetControlBrush();
}

Handle<Brush> PopupMenu::GetBorderBrush()
{
auto theme=GetTheme();
return theme->GetBorderBrush();
}

Graphics::SIZE PopupMenu::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
auto theme=GetTheme();
auto font=theme->GetDefaultFont();
UINT label_width=0;
UINT shortcut_width=0;
bool icon=false;
bool arrow=false;
bool separator=true;
Handle<UI::Window> last_sep;
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	auto item=Convert<PopupMenuItem>(child);
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
		label_width=MAX(label_width, size.Width);
		}
	auto shortcut=item->Shortcut;
	if(shortcut)
		{
		SIZE size=target->MeasureText(font, scale, shortcut->Begin());
		shortcut_width=MAX(shortcut_width, size.Width);
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
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	auto item=Convert<PopupMenuItem>(child);
	if(!item)
		continue;
	item->SetColumns(icon_width, label_width, shortcut_width);
	}
return StackPanel::GetMinSize(target);
}

VOID PopupMenu::Show(UI::Window* owner, POINT const& pt)
{
Popup(this);
pOwner=owner;
auto frame=GetFrame();
auto current=frame->GetCurrentMenu();
if(current)
	{
	if(!IsParentMenu(current))
		current->Close();
	}
frame->SetCurrentMenu(this);
BOOL keyboard=false;
if(pParentMenu)
	keyboard|=pParentMenu->HasKeyboardAccess();
SetFlag(uMenuFlags, MenuFlags::KeyboardAccess, keyboard);
rcRect.Left=pt.Left;
rcRect.Top=pt.Top;
Visible=true;
}

}}}