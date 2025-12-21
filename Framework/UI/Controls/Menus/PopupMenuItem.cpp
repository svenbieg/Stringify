//===================
// PopupMenuItem.cpp
//===================

#include "PopupMenuItem.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "UI/Controls/Menus/MenuBar.h"
#include "UI/Controls/Menus/MenuHelper.h"
#include "UI/Controls/Menus/PopupMenu.h"
#include "UI/Input/Shortcut.h"
#include "UI/Application.h"

using namespace Concurrency;
using namespace Graphics;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//========
// Common
//========

Graphics::SIZE PopupMenuItem::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
FLOAT scale=GetScaleFactor();
size.Width+=m_IconWidth;
size.Width+=m_LabelWidth;
size.Width+=m_ShortcutWidth;
if(Text)
	{
	auto font=m_Theme->DefaultFont;
	if(Icon)
		size.Height=16*scale;
	SIZE label_size=target->MeasureText(font, scale, Text->Begin());
	size.Height=TypeHelper::Max(size.Height, label_size.Height);
	if(Shortcut)
		{
		SIZE shortcut_size=target->MeasureText(font, scale, Shortcut->Begin());
		size.Height=TypeHelper::Max(size.Height, shortcut_size.Height);
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
FLOAT scale=GetScaleFactor();
if(!Text)
	{
	auto brush=m_Theme->BorderBrush;
	POINT from(0, Padding.Top*scale);
	POINT to(rc.Right, Padding.Top*scale);
	target->DrawLine(from, to, brush);
	return;
	}
rc.SetPadding(Padding*scale);
UINT height=rc.GetHeight();
UINT left=rc.Left;
if(Icon)
	{
	if(!m_Icon)
		m_Icon=Icon->GetBitmap(height);
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
	ico_size.Width=height*0.9f;
	ico_size.Height=height*0.9f;
	UINT ico_top=rc.Top+(height-ico_size.Height)/2;
	UINT ico_left=left-6*scale;
	RECT dst_rc(ico_left, ico_top, ico_left+ico_size.Width, ico_top+ico_size.Height);
	target->DrawBitmap(dst_rc, icon, ico_rc);
	}
left+=m_IconWidth;
auto font=m_Theme->DefaultFont;
auto text_brush=m_Theme->TextBrush;
if(!Enabled)
	text_brush=m_Theme->TextInactiveBrush;
auto label=Text->Begin();
SIZE label_size=target->MeasureText(font, scale, label);
UINT top=rc.Top+(height-label_size.Height)/2;
RECT label_rc(left, top, left+label_size.Width, top+label_size.Height);
target->DrawText(label_rc, scale, font, text_brush, label);
BOOL accelerate=Accelerator;
if(!Enabled)
	accelerate=false;
if(!m_Menu->HasAcceleration())
	accelerate=false;
if(accelerate)
	{
	UINT pos=0;
	if(StringHelper::FindChar(label, Accelerator, &pos, false))
		{
		SIZE size_from(0, 0);
		if(pos>0)
			size_from=target->MeasureText(font, scale, label, pos);
		SIZE size_to=target->MeasureText(font, scale, label, pos+1);
		POINT from(left+size_from.Width, rc.Bottom);
		POINT to(left+size_to.Width, rc.Bottom);
		target->DrawLine(from, to, text_brush, 1);
		}
	}
left+=m_LabelWidth;
if(Shortcut)
	{
	auto shortcut=Shortcut->Begin();
	SIZE shortcut_size=target->MeasureText(font, scale, shortcut);
	UINT top=rc.Top+(height-shortcut_size.Height)/2;
	RECT shortcut_rc(left, top, left+shortcut_size.Width, top+shortcut_size.Height);
	auto shortcut_brush=m_Theme->TextBrush;
	if(!Enabled)
		shortcut_brush=m_Theme->TextInactiveBrush;
	target->DrawText(shortcut_rc, scale, font, shortcut_brush, shortcut);
	}
if(SubMenu)
	{
	SIZE arrow_size=target->MeasureText(font, scale, TEXT(">"));
	UINT top=rc.Top+(height-arrow_size.Height)/2;
	RECT arrow_rc(rc.Right-arrow_size.Width, top, rc.Right, top+arrow_size.Height);
	auto arrow_brush=m_Theme->TextBrush;
	if(!Enabled)
		arrow_brush=m_Theme->TextInactiveBrush;
	target->DrawText(arrow_rc, scale, font, arrow_brush, TEXT(">"));
	}
}

VOID PopupMenuItem::SetColumns(UINT icon_width, UINT label_width, UINT shortcut_width)
{
m_IconWidth=icon_width;
m_LabelWidth=label_width;
m_ShortcutWidth=shortcut_width;
}


//==========================
// Con-/Destructors Private
//==========================

PopupMenuItem::PopupMenuItem(PopupMenu* parent, Handle<Sentence> label):
Interactive(parent->GetPanel()),
MenuItem(this, parent),
Label(this),
Padding(12, 3, 12, 3),
m_IconWidth(0),
m_LabelWidth(0),
m_ShortcutWidth(0)
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


//================
// Common Private
//================

VOID PopupMenuItem::DoClick()
{
Clicked(this);
}

VOID PopupMenuItem::OnClicked()
{
m_Menu->Exit();
DispatchedQueue::Append(this, &PopupMenuItem::DoClick);
}

VOID PopupMenuItem::OnLabelChanged(Handle<Sentence> label)
{
if(label)
	{
	Accelerator=MenuHelper::GetAccelerator(label->Begin());
	Shortcut=MenuHelper::GetShortcut(label->Begin());
	Text=MenuHelper::GetLabel(label->Begin());
	auto shortcut=ShortcutFromString(Shortcut);
	if(shortcut)
		Application::GetCurrent()->Shortcuts->Set(shortcut, this, false);
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
		auto control=Interactive::GetNextControl(m_Parent, this, 1);
		if(control)
			{
			auto item=dynamic_cast<MenuItem*>(control);
			m_Menu->Select(item);
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
		auto parent_menu=m_Menu->GetParentMenu();
		auto menubar=dynamic_cast<MenuBar*>(parent_menu->GetPanel());
		if(menubar)
			{
			auto control=Interactive::GetNextControl(m_Parent, nullptr, 0);
			if(control==this)
				{
				m_Menu->Close();
				return;
				}
			}
		auto control=Interactive::GetNextControl(m_Parent, this, -1);
		if(control)
			{
			auto item=dynamic_cast<MenuItem*>(control);
			m_Menu->Select(item);
			}
		return;
		}
	default:
		break;
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
m_Menu->KillKeyboardAccess();
auto app=Application::GetCurrent();
auto current=app->GetCurrentMenu();
if(current==SubMenu)
	return;
auto menu=m_Menu;
while(current!=menu)
	{
	current->Close();
	current=app->GetCurrentMenu();
	}
if(SubMenu)
	{
	if(!SubMenu->Visible)
		{
		m_Timer=Timer::Create();
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