//=================
// MenuBarItem.cpp
//=================

#include "MenuBarItem.h"


//=======
// Using
//=======

#include "UI/Controls/Menus/MenuBar.h"
#include "UI/Controls/Menus/MenuHelper.h"
#include "UI/Controls/Menus/PopupMenu.h"

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

Handle<Brush> MenuBarItem::GetBackground()
{
auto brush=Background;
if(!IsEnabled())
	return brush;
BOOL has_focus=HasFocus();
has_focus|=HasPointerFocus();
if(SubMenu)
	{
	if(SubMenu->Visible)
		has_focus=true;
	}
if(has_focus)
	brush=Highlight;
return brush;
}

Graphics::SIZE MenuBarItem::GetMinSize(RenderTarget* target)
{
auto font=m_Theme->DefaultFont;
FLOAT scale=GetScaleFactor();
SIZE size=target->MeasureText(font, scale, Text->Begin());
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID MenuBarItem::Render(RenderTarget* target, RECT& rc)
{
Interactive::Render(target, rc);
BOOL enabled=IsEnabled();
auto font=m_Theme->DefaultFont;
auto text_brush=m_Theme->TextBrush;
if(!enabled)
	text_brush=m_Theme->TextInactiveBrush;
FLOAT scale=GetScaleFactor();
RECT rc_text=rc;
rc_text.SetPadding(Padding*scale);
auto text=Text->Begin();
target->DrawText(rc_text, scale, font, text_brush, text);
BOOL accelerate=(Accelerator!=0);
if(!enabled)
	accelerate=false;
if(!m_Menu->Acceleration())
	accelerate=false;
if(accelerate)
	{
	UINT pos=0;
	if(StringHelper::FindChar(text, Accelerator, &pos, CompareMode::IgnoreCase))
		{
		SIZE size_from(0, 0);
		if(pos>0)
			size_from=target->MeasureText(font, scale, text, pos);
		SIZE size_to=target->MeasureText(font, scale, text, pos+1);
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
Background=m_Theme->ControlBrush;
Focused.Add(this, &MenuBarItem::OnFocused);
FocusLost.Add(this, &MenuBarItem::OnFocusLost);
Highlight=m_Theme->HighlightBrush;
Label.Changed.Add(this, &MenuBarItem::OnLabelChanged);
Label=label;
KeyDown.Add(this, &MenuBarItem::OnKeyDown);
KeyPressed.Add(this, &MenuBarItem::OnKeyPressed);
PointerDown.Add(this, &MenuBarItem::OnPointerDown);
PointerEntered.Add(this, &MenuBarItem::OnPointerEntered);
TabStop=true;
}


//================
// Common Private
//================

VOID MenuBarItem::OnFocused(Interactive* previous, FocusReason reason)
{
m_Menu->Select(this, reason);
}

VOID MenuBarItem::OnFocusLost(Interactive* focus, FocusReason reason)
{
auto item=dynamic_cast<MenuItem*>(focus);
if(item)
	return;
m_Menu->ClearSelection(this, reason);
}

VOID MenuBarItem::OnKeyDown(Handle<KeyEventArgs> args)
{
if(args->Handled)
	return;
switch(args->Key)
	{
	case VirtualKey::Escape:
		{
		m_Menu->Escape(FocusReason::Keyboard);
		break;
		}
	case VirtualKey::Down:
		{
		Expand(FocusReason::Keyboard);
		break;
		}
	default:
		{
		return;
		}
	}
args->Handled=true;
}

VOID MenuBarItem::OnKeyPressed(Handle<KeyEventArgs> args)
{
switch(args->Key)
	{
	case VirtualKey::Return:
		{
		Expand(FocusReason::Keyboard);
		break;
		}
	default:
		{
		return;
		}
	}
args->Handled=true;
}

VOID MenuBarItem::OnLabelChanged(Handle<Sentence> label)
{
Accelerator=MenuHelper::GetAccelerator(label->Begin());
Text=MenuHelper::GetText(label->Begin());
Invalidate(true);
}

VOID MenuBarItem::OnPointerDown()
{
m_Menu->Expand(this, FocusReason::Pointer);
}

VOID MenuBarItem::OnPointerEntered()
{
m_Menu->Select(this, FocusReason::Pointer);
}

}}}