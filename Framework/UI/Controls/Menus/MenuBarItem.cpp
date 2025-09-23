//=================
// MenuBarItem.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "MenuBar.h"
#include "MenuHelper.h"
#include "PopupMenu.h"

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

Handle<PopupMenuItem> MenuBarItem::Add(Handle<Sentence> label)
{
if(!SubMenu)
	SubMenu=PopupMenu::Create(m_Menu);
return SubMenu->Add(label);
}

Graphics::SIZE MenuBarItem::GetMinSize(RenderTarget* target)
{
auto font=m_Theme->DefaultFont;
FLOAT scale=GetScaleFactor();
SIZE size=target->MeasureText(font, scale, m_Label->Begin());
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID MenuBarItem::Render(RenderTarget* target, RECT& rc)
{
Interactive::Render(target, rc);
auto font=m_Theme->DefaultFont;
auto text_brush=m_Theme->TextBrush;
if(!Enabled)
	text_brush=m_Theme->TextInactiveBrush;
FLOAT scale=GetScaleFactor();
RECT rc_text=rc;
rc_text.SetPadding(Padding*scale);
auto label=m_Label->Begin();
target->DrawText(rc_text, scale, font, text_brush, label);
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
		POINT from(rc_text.Left+size_from.Width, rc_text.Bottom);
		POINT to(rc_text.Left+size_to.Width, rc_text.Bottom);
		target->DrawLine(from, to, text_brush, 1);
		}
	}
}


//==========================
// Con-/Destructors Private
//==========================

MenuBarItem::MenuBarItem(MenuBar* parent, Handle<Sentence> label):
Interactive(parent->GetPanel()),
MenuItem(this, parent),
Label(this),
Padding(6, 2, 6, 2)
{
Label.Changed.Add(this, &MenuBarItem::OnLabelChanged);
Label=label;
KeyDown.Add(this, &MenuBarItem::OnKeyDown);
PointerDown.Add(this, &MenuBarItem::OnPointerDown);
PointerEntered.Add(this, &MenuBarItem::OnPointerEntered);
PointerLeft.Add(this, &MenuBarItem::OnPointerLeft);
}


//================
// Common Private
//================

VOID MenuBarItem::OnKeyDown(Handle<KeyEventArgs> args)
{
MenuItem::OnKeyDown(args);
if(args->Handled)
	return;
args->Handled=true;
switch(args->Key)
	{
	case VirtualKey::Down:
		{
		m_Menu->Open(this);
		return;
		}
	case VirtualKey::Left:
		{
		auto control=Interactive::GetNextControl(m_Parent, this, -1);
		if(control)
			{
			auto item=dynamic_cast<MenuItem*>(control);
			m_Menu->Select(item);
			}
		return;
		}
	case VirtualKey::Return:
		{
		m_Menu->Open(this);
		return;
		}
	case VirtualKey::Right:
		{
		auto control=Interactive::GetNextControl(m_Parent, this, -1);
		if(control)
			{
			auto item=dynamic_cast<MenuItem*>(control);
			m_Menu->Select(item);
			}
		return;
		}
	case VirtualKey::Up:
		{
		Close();
		return;
		}
	default:
		break;
	}
args->Handled=false;
}

VOID MenuBarItem::OnLabelChanged(Handle<Sentence> label)
{
Accelerator=MenuHelper::GetAccelerator(label->Begin());
m_Label=MenuHelper::GetLabel(label->Begin());
Invalidate(true);
}

VOID MenuBarItem::OnPointerDown()
{
m_Menu->KillKeyboardAccess();
m_Menu->Switch(this);
}

VOID MenuBarItem::OnPointerEntered()
{
m_Menu->KillKeyboardAccess();
m_Menu->Select(this);
Invalidate();
}

VOID MenuBarItem::OnPointerLeft()
{
Invalidate();
}

}}}