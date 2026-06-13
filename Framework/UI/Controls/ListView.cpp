//==============
// ListView.cpp
//==============

#include "ListView.h"


//=======
// Using
//=======

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Handle<Brush> ListView::GetBackground()
{
if(!m_Theme)
	return nullptr;
auto brush=m_Theme->WindowBrush;
if(!IsEnabled())
	brush=m_Theme->ControlBrush;
return brush;
}

Graphics::SIZE ListView::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
UINT columns=Columns;
FLOAT scale=GetScaleFactor();
m_ItemSize=ItemSize;
if(m_ItemSize.Height==0)
	m_ItemSize.Height=GetLineHeight(target, 1.f);
UINT item_count=0;
if(Source)
	item_count=Source->GetChildCount();
UINT line_count=item_count/columns;
UINT line_height=m_ItemSize.Height*scale;
size.Height=line_count*line_height;
size.Width=m_ItemSize.Width*scale;
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID ListView::Render(RenderTarget* target, RECT& rc)
{
auto background=GetBackground();
RECT rc_fill(rc);
if(background)
	target->FillRect(rc_fill, background);
if(!Source)
	return;
auto offset=target->GetOffset();
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
UINT client_height=rc.GetHeight();
UINT columns=Columns;
UINT item_height=m_ItemSize.Height*scale;
UINT item_width=m_ItemSize.Width*scale;
UINT first_line=offset.Top/item_height;
UINT line_count=client_height/item_height+2;
UINT last_line=first_line+line_count-1;
BOOL show_sel=true;
if(m_SelectionFirst==m_SelectionLast)
	show_sel=false;
if(show_sel)
	{
	auto highlight=m_Theme->HighlightBrush;
	if(!HasFocus())
		highlight=m_Theme->HighlightInactiveBrush;
	UINT sel_first=m_SelectionFirst;
	if((sel_first/columns)<first_line)
		sel_first=first_line*columns;
	UINT sel_last=m_SelectionLast;
	if((sel_last/columns)>last_line)
		sel_last=(last_line+1)*columns;
	POINT pt_first=GetItemPosition(sel_first, scale);
	POINT pt_last=GetItemPosition(sel_last, scale);
	if(pt_first.Top==pt_last.Top)
		{
		RECT rc_fill(pt_first.Left, pt_first.Top, pt_last.Left, pt_last.Top+item_height);
		target->FillRect(rc_fill, highlight);
		}
	else
		{
		UINT right=rc.Left+columns*item_width;
		RECT rc_first(pt_first.Left, pt_first.Top, right, pt_first.Top+item_height);
		target->FillRect(rc_fill, highlight);
		UINT full_lines=(sel_last-sel_first)/columns;
		if(full_lines)
			{
			RECT rc_full(0, pt_first.Top+item_height, right, pt_first.Top+full_lines*item_height);
			target->FillRect(rc_full, highlight);
			}
		RECT rc_last(rc.Left, pt_last.Top, pt_last.Left, pt_last.Top+item_height);
		target->FillRect(rc_last, highlight);
		}
	}
auto font=m_Theme->DefaultFont;
auto brush=m_Theme->TextBrush;
UINT first_item=first_line*columns;
UINT top=rc.Top+first_line*item_height;
auto children=Source->GetChildren();
children->SetPosition(first_item);
for(UINT line=first_line; line<last_line; line++)
	{
	UINT left=rc.Left;
	for(UINT col=0; col<columns; col++)
		{
		if(!children->HasCurrent())
			break;
		auto item=children->GetCurrent();
		auto text=item->GetAttribute("Name");
		if(text)
			{
			RECT rc_text(left, top, left+item_width, top+item_height);
			target->DrawText(rc_text, scale, font, brush, text->Begin());
			}
		}
	}
}


//============================
// Con-/Destructors Protected
//============================

ListView::ListView(Window* parent):
Interactive(parent),
Columns(this, 1),
ItemSize(this, SIZE(200, 0)),
Source(this),
m_SelectionEnd(0),
m_SelectionFirst(0),
m_SelectionLast(0),
m_SelectionStart(0)
{
Columns.Changed.Add(this, &ListView::OnColumnsChanged);
ItemSize.Changed.Add(this, &ListView::OnItemSizeChanged);
Source.Changed.Add(this, &ListView::OnSourceChanged);
}


//================
// Common Private
//================

Graphics::POINT ListView::GetItemPosition(UINT item, FLOAT scale)
{
UINT item_width=m_ItemSize.Width*scale;
UINT item_height=m_ItemSize.Height*scale;
UINT columns=Columns;
UINT line=item/columns;
UINT col=item%columns;
return POINT(col*item_width, line*item_height);
}

UINT ListView::GetLineHeight(RenderTarget* target, FLOAT scale)
{
auto font=m_Theme->DefaultFont;
SIZE size=target->MeasureText(font, scale, TEXT("Ag"), 2);
return size.Height;
}

VOID ListView::OnColumnsChanged()
{
Invalidate(true);
}

VOID ListView::OnItemSizeChanged()
{
Invalidate(true);
}

VOID ListView::OnSourceChanged()
{
Invalidate(true);
}

}}