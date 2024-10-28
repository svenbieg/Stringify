//===================
// PopupMenuItem.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#include "UI/Input/Shortcut.h"
#include "UI/Application.h"
#include "MenuBar.h"
#include "MenuHelper.h"
#include "PopupMenu.h"

using namespace Graphics;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

PopupMenuItem::PopupMenuItem(PopupMenu* menu, Handle<Sentence> label):
Interactive(menu->GetPanel()),
MenuItem(this, menu),
Label(this),
Padding(12, 3, 12, 3),
uIconWidth(0),
uLabelWidth(0),
uShortcutWidth(0)
{
Interactive::Clicked.Add(this, &PopupMenuItem::OnClicked);
if(!label)
	Enabled=false;
Label.Changed.Add(this, &PopupMenuItem::OnLabelChanged);
Label=label;
KeyDown.Add(this, &PopupMenuItem::OnKeyDown);
PointerDown.Add(this, &PopupMenuItem::OnPointerDown);
PointerEntered.Add(this, &PopupMenuItem::OnPointerEntered);
PointerLeft.Add(this, &PopupMenuItem::OnPointerLeft);
}


//========
// Common
//========

Handle<Brush> PopupMenuItem::GetBackgroundBrush()
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

Graphics::SIZE PopupMenuItem::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
FLOAT scale=GetScaleFactor();
size.Width+=uIconWidth;
size.Width+=uLabelWidth;
size.Width+=uShortcutWidth;
if(Text)
	{
	if(Icon)
		size.Height=16*scale;
	auto font=GetFont();
	SIZE label_size=target->MeasureText(font, scale, Text->Begin());
	size.Height=Max(size.Height, label_size.Height);
	if(Shortcut)
		{
		SIZE shortcut_size=target->MeasureText(font, scale, Shortcut->Begin());
		size.Height=Max(size.Height, shortcut_size.Height);
		}
	}
else
	{
	size.Height=1;
	}
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID PopupMenuItem::Render(RenderTarget* target, RECT& rc)
{
Interactive::Render(target, rc);
auto theme=GetTheme();
FLOAT scale=GetScaleFactor();
if(!Text)
	{
	POINT from(0, Padding.Top*scale);
	POINT to(rc.Right, Padding.Top*scale);
	target->DrawLine(from, to, theme->LightBorderBrush);
	return;
	}
rc.SetPadding(Padding*scale);
UINT height=rc.GetHeight();
UINT left=rc.Left;
if(Icon)
	{
	if(!hIcon)
		hIcon=Icon->GetBitmap(height);
	auto icon=hIcon;
	if(!Enabled)
		{
		if(!hIconDisabled)
			{
			hIconDisabled=hIcon->Copy();
			DisableMenuBitmap(hIconDisabled);
			}
		icon=hIconDisabled;
		}
	SIZE ico_size=icon->GetDimensions();
	RECT ico_rc(ico_size);
	ico_size.Width=height*0.9f;
	ico_size.Height=height*0.9f;
	UINT ico_top=rc.Top+(height-ico_size.Height)/2;
	UINT ico_left=left-6*scale;
	RECT dst_rc(ico_left, ico_top, ico_left+ico_size.Width, ico_top+ico_size.Height);
	target->DrawBitmap(dst_rc, icon, ico_rc);
	}
left+=uIconWidth;
auto font=GetFont();
auto text_color=theme->TextBrush;
if(!Enabled)
	text_color=theme->BorderBrush;
auto label=Text->Begin();
SIZE label_size=target->MeasureText(font, scale, label);
UINT top=rc.Top+(height-label_size.Height)/2;
RECT label_rc(left, top, left+label_size.Width, top+label_size.Height);
target->Font=font;
target->TextColor=text_color;
target->DrawText(label_rc, scale, label);
BOOL accelerate=Accelerator;
if(!Enabled)
	accelerate=false;
if(!pMenu->HasAcceleration())
	accelerate=false;
if(accelerate)
	{
	UINT pos=0;
	if(StringFindChar(label, Accelerator, &pos, false))
		{
		SIZE size_from(0, 0);
		if(pos>0)
			size_from=target->MeasureText(font, scale, label, pos);
		SIZE size_to=target->MeasureText(font, scale, label, pos+1);
		POINT from(left+size_from.Width, rc.Bottom);
		POINT to(left+size_to.Width, rc.Bottom);
		target->DrawLine(from, to, text_color, 1);
		}
	}
left+=uLabelWidth;
if(Shortcut)
	{
	auto shortcut=Shortcut->Begin();
	SIZE shortcut_size=target->MeasureText(font, scale, shortcut);
	UINT top=rc.Top+(height-shortcut_size.Height)/2;
	RECT shortcut_rc(left, top, left+shortcut_size.Width, top+shortcut_size.Height);
	auto shortcut_color=theme->GrayTextBrush;
	if(!Enabled)
		shortcut_color=theme->LightTextBrush;
	target->TextColor=shortcut_color;
	target->DrawText(shortcut_rc, scale, shortcut);
	}
if(SubMenu)
	{
	SIZE arrow_size=target->MeasureText(font, scale, TEXT(">"));
	UINT top=rc.Top+(height-arrow_size.Height)/2;
	RECT arrow_rc(rc.Right-arrow_size.Width, top, rc.Right, top+arrow_size.Height);
	auto arrow_color=theme->GrayTextBrush;
	if(!Enabled)
		arrow_color=theme->LightTextBrush;
	target->TextColor=arrow_color;
	target->DrawText(arrow_rc, scale, TEXT(">"));
	}
}

VOID PopupMenuItem::SetColumns(UINT icon_width, UINT label_width, UINT shortcut_width)
{
uIconWidth=icon_width;
uLabelWidth=label_width;
uShortcutWidth=shortcut_width;
}


//================
// Common Private
//================

VOID PopupMenuItem::DoClick()
{
Clicked(this);
}

VOID PopupMenuItem::OnClicked()
{
pMenu->Exit();
Application::Current->Dispatch(this, &PopupMenuItem::DoClick);
}

VOID PopupMenuItem::OnLabelChanged(Handle<Sentence> label)
{
if(label)
	{
	Accelerator=GetMenuAccelerator(label->Begin());
	Shortcut=GetMenuShortcut(label->Begin());
	Text=GetMenuLabel(label->Begin());
	auto shortcut=ShortcutFromString(Shortcut);
	if(shortcut)
		Application::Current->Shortcuts->Set(shortcut, this, false);
	Enabled=true;
	}
else
	{
	Enabled=false;
	}
Invalidate();
}

VOID PopupMenuItem::OnKeyDown(Handle<KeyEventArgs> args)
{
MenuItem::OnKeyDown(args);
if(args->Handled)
	return;
args->Handled=true;
switch(args->Key)
	{
	case VirtualKey::Down:
		{
		auto control=Interactive::GetNextControl(Parent, this, 1);
		if(control)
			{
			auto item=Convert<MenuItem>(control);
			pMenu->Select(item);
			}
		return;
		}
	case VirtualKey::Return:
		{
		Open();
		return;
		}
	case VirtualKey::Right:
		{
		return;
		}
	case VirtualKey::Up:
		{
		auto parent_menu=pMenu->GetParentMenu();
		auto menubar=Convert<MenuBar>(parent_menu->GetPanel());
		if(menubar)
			{
			auto control=Interactive::GetNextControl(Parent, nullptr, 0);
			if(control==this)
				{
				pMenu->Close();
				return;
				}
			}
		auto control=Interactive::GetNextControl(Parent, this, -1);
		if(control)
			{
			auto item=Convert<MenuItem>(control);
			pMenu->Select(item);
			}
		return;
		}
	}
args->Handled=false;
}

VOID PopupMenuItem::OnPointerDown()
{
if(SubMenu)
	{
	if(!SubMenu->Visible)
		{
		Open();
		}
	else
		{
		SubMenu->Close();
		}
	}
}

VOID PopupMenuItem::OnPointerEntered()
{
Invalidate();
pMenu->KillKeyboardAccess();
auto current=Application::Current->GetCurrentMenu();
if(current==SubMenu)
	return;
auto menu=pMenu;
while(current!=menu)
	{
	current->Close();
	current=Application::Current->GetCurrentMenu();
	}
if(SubMenu)
	{
	if(!SubMenu->Visible)
		{
		m_Timer=new Timer();
		m_Timer->Triggered.Add(this, &PopupMenuItem::OnTimerTriggered);
		m_Timer->StartOnce(300);
		}
	}
}

VOID PopupMenuItem::OnPointerLeft()
{
Invalidate();
m_Timer=nullptr;
}

VOID PopupMenuItem::OnTimerTriggered()
{
m_Timer=nullptr;
Open();
}

}}}