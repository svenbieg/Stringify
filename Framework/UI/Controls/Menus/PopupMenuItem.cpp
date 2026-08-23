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
using namespace Resources::Icons;
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

Handle<PopupMenuItem> PopupMenuItem::Add(Handle<Sentence> label)
{
if(!SubMenu)
	SubMenu=PopupMenu::Create(m_Menu);
return SubMenu->Add(label);
}

Handle<Brush> PopupMenuItem::GetBackground()
{
auto brush=Background;
if(IsEnabled())
	{
	BOOL has_focus=HasFocus();
	has_focus|=HasPointerFocus();
	if(has_focus)
		brush=Highlight;
	}
return brush;
}

Graphics::SIZE PopupMenuItem::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
FLOAT scale=GetScaleFactor();
if(m_SymbolWidth)
	size.Width+=m_SymbolWidth+SYMBOL_PADDING*scale;
size.Width+=m_LabelWidth;
if(m_ShortcutWidth)
	size.Width+=m_ShortcutWidth+SHORTCUT_PADDING*scale;
if(Text)
	{
	auto font=m_Theme->DefaultFont;
	size.Height=font->GetSize(target, scale);
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
BOOL enabled=IsEnabled();
auto font=m_Theme->DefaultFont;
auto text_brush=m_Theme->TextBrush;
if(!enabled)
	text_brush=m_Theme->TextInactiveBrush;
if(Icon)
	{
	if(!m_Icon)
		m_Icon=Icon->GetBitmap(height);
	auto icon=m_Icon;
	if(!enabled)
		{
		if(!m_IconDisabled)
			m_IconDisabled=MenuHelper::GetBitmapDisabled(m_Icon);
		icon=m_IconDisabled;
		}
	SIZE ico_size=icon->GetDimensions();
	RECT ico_rc(ico_size);
	ico_size.Width=m_SymbolWidth;
	ico_size.Height=m_SymbolWidth;
	UINT ico_left=left+(m_SymbolWidth-ico_size.Width)/2;
	UINT ico_top=rc.Top+(height-ico_size.Height)/2;
	RECT dst_rc(ico_left, ico_top, ico_left+ico_size.Width, ico_top+ico_size.Height);
	target->DrawBitmap(dst_rc, icon, ico_rc);
	}
else if(Symbol)
	{
	auto symbol=Symbol->Begin();
	SIZE text_size=target->MeasureText(font, scale, symbol);
	UINT sym_left=left+(m_SymbolWidth-text_size.Width)/2;
	UINT sym_top=rc.Top+(height-text_size.Height)/2;
	RECT text_rc(sym_left, sym_top, sym_left+text_size.Width, sym_top+text_size.Height);
	target->DrawText(text_rc, scale, font, text_brush, symbol);
	}
if(m_SymbolWidth)
	left+=m_SymbolWidth+SYMBOL_PADDING*scale;
auto text=Text->Begin();
SIZE text_size=target->MeasureText(font, scale, text);
UINT top=rc.Top+(height-text_size.Height)/2;
RECT text_rc(left, top, left+text_size.Width, top+text_size.Height);
target->DrawText(text_rc, scale, font, text_brush, text);
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
		POINT from(left+size_from.Width, rc.Bottom);
		POINT to(left+size_to.Width, rc.Bottom);
		target->DrawLine(from, to, text_brush, 1);
		}
	}
left+=m_LabelWidth;
if(m_ShortcutWidth)
	left+=SHORTCUT_PADDING*scale;
if(Shortcut)
	{
	auto shortcut=Shortcut->Begin();
	SIZE shortcut_size=target->MeasureText(font, scale, shortcut);
	UINT top=rc.Top+(height-shortcut_size.Height)/2;
	RECT shortcut_rc(left, top, left+shortcut_size.Width, top+shortcut_size.Height);
	auto shortcut_brush=m_Theme->TextBrush;
	if(!enabled)
		shortcut_brush=m_Theme->TextInactiveBrush;
	target->DrawText(shortcut_rc, scale, font, shortcut_brush, shortcut);
	}
else if(SubMenu)
	{
	UINT height=rc.Bottom-rc.Top;
	UINT padding=6*scale;
	RECT arrow_rc(rc);
	arrow_rc.Left=arrow_rc.Right-height;
	POINT pts[3];
	pts[0].Set(arrow_rc.Left+padding, arrow_rc.Top+padding);
	pts[1].Set(arrow_rc.Left+padding, arrow_rc.Bottom-padding);
	pts[2].Set(arrow_rc.Right-padding, arrow_rc.Top+height/2);
	target->FillPolygon(pts, 3, text_brush);
	}
}

VOID PopupMenuItem::SetColumns(UINT symbol_width, UINT label_width, UINT shortcut_width)
{
m_SymbolWidth=symbol_width;
m_LabelWidth=label_width;
m_ShortcutWidth=shortcut_width;
}


//==========================
// Con-/Destructors Private
//==========================

PopupMenuItem::PopupMenuItem(PopupMenu* parent, Handle<Sentence> label):
Interactive(parent->GetPanel()),
MenuItem(this, parent),
Checked(this, false),
Label(this),
Padding(12, 3, 12, 3),
m_LabelWidth(0),
m_ShortcutWidth(0),
m_SymbolWidth(0)
{
if(!label)
	{
	Enabled=false;
	return;
	}
Interactive::Clicked.Add(this, &PopupMenuItem::OnInteractiveClicked);
Background=m_Theme->ControlBrush;
Checked.Changed.Add(this, &PopupMenuItem::OnCheckedChanged);
Focused.Add(this, &PopupMenuItem::OnFocused);
Highlight=m_Theme->HighlightBrush;
Label.Changed.Add(this, &PopupMenuItem::OnLabelChanged);
Label=label;
KeyDown.Add(this, &PopupMenuItem::OnKeyDown);
PointerDown.Add(this, &PopupMenuItem::OnPointerDown);
PointerEntered.Add(this, &PopupMenuItem::OnPointerEntered);
PointerLeft.Add(this, &PopupMenuItem::OnPointerLeft);
TabStop=true;
}


//================
// Common Private
//================

VOID PopupMenuItem::OnCheckedChanged(BOOL checked)
{
if(checked)
	{
	Symbol=L"\x2713";
	}
else
	{
	Symbol=L" ";
	}
}

VOID PopupMenuItem::OnFocused(Interactive* previous, FocusReason reason)
{
m_Menu->Select(this, reason);
}

VOID PopupMenuItem::OnInteractiveClicked()
{
if(!SubMenu)
	{
	m_Menu->Exit(FocusReason::Pointer);
	DispatchedQueue::Append(this, [this](){ Clicked(this); });
	}
}

VOID PopupMenuItem::OnLabelChanged(Handle<Sentence> label)
{
if(label)
	{
	Accelerator=MenuHelper::GetAccelerator(label->Begin());
	Shortcut=MenuHelper::GetShortcut(label->Begin());
	Text=MenuHelper::GetText(label->Begin());
	auto shortcut=Shortcut::FromString(Shortcut);
	if(shortcut)
		Application::GetCurrent()->Shortcuts->Set(shortcut, this, EventNotification::None);
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
		m_Frame->FocusNext(FocusReason::Keyboard);
		break;
		}
	case VirtualKey::Left:
		{
		m_Menu->Escape(FocusReason::Keyboard);
		break;
		}
	case VirtualKey::Right:
		{
		Expand(FocusReason::Keyboard);
		break;
		}
	case VirtualKey::Up:
		{
		auto control=Interactive::GetNextControl(m_Parent, nullptr, true);
		if(control==this)
			{
			auto parent_menu=m_Menu->GetParentMenu();
			auto menubar=dynamic_cast<MenuBar*>(parent_menu);
			if(menubar)
				{
				m_Menu->Escape(FocusReason::Keyboard);
				break;
				}
			}
		m_Frame->FocusNext(FocusReason::Keyboard, false);
		break;
		}
	default:
		{
		auto item=m_Menu->Accelerate(args->Key);
		if(item)
			{
			m_Menu->Enter(item, FocusReason::Keyboard);
			break;
			}
		return;
		}
	}
args->Handled=true;
}

VOID PopupMenuItem::OnKeyPressed(Handle<KeyEventArgs> args)
{
switch(args->Key)
	{
	case VirtualKey::Return:
		{
		Enter(FocusReason::Keyboard);
		break;
		}
	default:
		{
		return;
		}
	}
args->Handled=true;
}

VOID PopupMenuItem::OnPointerDown()
{
m_Timer=nullptr;
Enter(FocusReason::Pointer);
}

VOID PopupMenuItem::OnPointerEntered()
{
m_Menu->Select(this, FocusReason::Pointer);
if(!SubMenu)
	return;
if(SubMenu->Visible)
	return;
m_Timer=Timer::Create();
m_Timer->Triggered.Add(this, &PopupMenuItem::OnTimerTriggered);
m_Timer->StartOnce(300);
}

VOID PopupMenuItem::OnPointerLeft()
{
m_Timer=nullptr;
Invalidate();
}

VOID PopupMenuItem::OnTimerTriggered()
{
m_Timer=nullptr;
Expand(FocusReason::Pointer);
}

}}}