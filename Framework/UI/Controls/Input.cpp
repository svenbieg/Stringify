//===========
// Input.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "Storage/Clipboard.h"
#include "Storage/Streams/StringReader.h"
#include "Storage/StaticBuffer.h"
#include "UI/Controls/Menus/EditMenu.h"
#include "UI/Frame.h"
#include "Input.h"

using namespace Clusters;
using namespace Core;
using namespace Graphics;
using namespace Storage;
using namespace Storage::Streams;
using namespace UI::Controls::Menus;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Input::Input(Window* parent):
Interactive(parent),
Mask(0),
MultiLine(false),
ReadOnly(false),
ptCursor(-1, -1),
ptPointer(-1, -1),
ptSelectionEnd(-1, -1),
ptSelectionFirst(-1, -1),
ptSelectionLast(-1, -1),
ptSelectionStart(-1, -1),
uCursorTime(0),
uInputFlags(InputFlags::None),
uLineHeight(0)
{
auto frame=GetFrame();
ContextMenu=new EditMenu(frame);
Focused.Add(this, &Input::OnFocused);
FocusLost.Add(this, &Input::OnFocusLost);
KeyDown.Add(this, &Input::OnKeyDown);
KeyUp.Add(this, &Input::OnKeyUp);
PointerDown.Add(this, &Input::OnPointerDown);
PointerMoved.Add(this, &Input::OnPointerMoved);
PointerUp.Add(this, &Input::OnPointerUp);
}


//========
// Common
//========

VOID Input::AppendLine(Handle<String> text)
{
INPUT_LINE& line=cLines.append();
line.Text=text;
UpdateLine(line);
Invalidate(true);
}

VOID Input::Clear()
{
ClearSelection();
cLines.clear();
cLineWidths.clear();
}

VOID Input::ClearSelection()
{
BOOL changed=ptSelectionStart.Left>=0;
ptSelectionStart.Set(-1, -1);
ptSelectionEnd.Set(-1, -1);
if(changed)
	UpdateSelection();
}

Handle<Brush> Input::GetBackgroundBrush()
{
auto theme=GetTheme();
if(!IsEnabled())
	return theme->GetControlBrush();
return theme->GetWindowBrush();
}

Handle<Cursor> Input::GetCursor()
{
auto theme=GetTheme();
return theme->GetTextCursor();
}

Graphics::RECT Input::GetCursorRect()
{
FLOAT scale=GetScaleFactor();
POINT pt_cursor=PointFromChar(ptCursor, scale);
if(GetFlag(uInputFlags, InputFlags::Pointing))
	pt_cursor=ptPointer;
UINT line_height=uLineHeight*scale;
RECT rc_cursor(pt_cursor.Left, pt_cursor.Top, pt_cursor.Left, pt_cursor.Top+line_height);
rc_cursor.SetMargin(Padding*scale);
return rc_cursor;
}

UINT Input::GetLineHeight()const
{
FLOAT scale=GetScaleFactor();
return uLineHeight*scale;
}

Graphics::SIZE Input::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
SIZE size(0, 0);
UINT width_count=cLineWidths.get_count();
if(width_count>0)
	{
	size.Width=cLineWidths.get_at(width_count-1).get_key();
	size.Width*=scale;
	}
uLineHeight=GetLineHeight(target, 1.f);
UINT line_count=MAX(cLines.get_count(), 1);
UINT line_height=uLineHeight*scale;
size.Height=line_count*line_height;
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

Handle<String> Input::GetSelection()
{
UINT len=GetTextLength(ptSelectionFirst, ptSelectionLast);
if(!len)
	return nullptr;
Handle<String> text=new String(len, nullptr);
auto buf=const_cast<LPTSTR>(text->Begin());
GetText(ptSelectionFirst, ptSelectionLast, buf, len+1);
return text;
}

Handle<String> Input::GetText()
{
POINT pt_start(0, 0);
POINT pt_end=GetEndPoint();
UINT len=GetTextLength(pt_start, pt_end);
if(!len)
	return nullptr;
Handle<String> text=new String(len, nullptr);
auto buf=const_cast<LPTSTR>(text->Begin());
GetText(pt_start, pt_end, buf, len+1);
return text;
}

VOID Input::ReadFromStream(InputStream* stream)
{
Clear();
StringReader reader(stream);
while(1)
	{
	Handle<String> line=reader.ReadString(nullptr, "\r\n", "\n");
	if(!line)
		break;
	AppendLine(line);
	if(reader.LastChar==0)
		break;
	if(!MultiLine)
		break;
	}
}

VOID Input::Render(RenderTarget* target, RECT& rc)
{
Interactive::Render(target, rc);
auto theme=GetTheme();
target->Font=GetFont();
target->TextColor=theme->GetTextBrush();
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
UINT client_height=rc.GetHeight();
POINT offset=target->GetOffset();
UINT line_height=uLineHeight*scale;
UINT first_line=offset.Top/line_height;
UINT line_count=client_height/line_height+2;
UINT last_line=first_line+line_count-1;
if(ptSelectionFirst!=ptSelectionLast)
	{
	auto highlight=theme->GetHighlightBrush();
	if(!HasFocus())
		highlight=theme->GetInactiveHighlightBrush();
	POINT pt_first=PointFromChar(ptSelectionFirst, scale);
	POINT pt_last=PointFromChar(ptSelectionLast, scale);
	if(ptSelectionFirst.Top==ptSelectionLast.Top)
		{
		RECT rc_fill(pt_first.Left, pt_first.Top, pt_last.Left, pt_last.Top+line_height);
		target->FillRect(rc_fill, highlight);
		}
	else
		{
		auto it=cLines.cbegin(ptSelectionFirst.Top);
		UINT right=rc.Left+GetLineWidth(it.get_current(), scale);
		RECT rc_first(pt_first.Left, pt_first.Top, right, pt_first.Top+line_height);
		target->FillRect(rc_first, highlight);
		for(UINT line=ptSelectionFirst.Top+1; line<ptSelectionLast.Top; line++)
			{
			if(line>last_line)
				break;
			it.move_next();
			if(line<first_line)
				continue;
			right=rc.Left+GetLineWidth(it.get_current(), scale);
			UINT line_top=rc.Top+line*line_height;
			RECT rc_fill(rc.Left, line_top, right, line_top+line_height);
			target->FillRect(rc_fill, highlight);
			}
		RECT rc_last(rc.Left, pt_last.Top, pt_last.Left, pt_last.Top+line_height);
		target->FillRect(rc_last, highlight);
		}
	}
UINT top=rc.Top+first_line*line_height;
UINT line=0;
for(auto it=cLines.cbegin(first_line); it.has_current(); it.move_next(), line++)
	{
	if(line>last_line)
		break;
	auto text=it->Text;
	if(text)
		{
		RECT rc_text(rc.Left, top, rc.Right, top+line_height);
		target->DrawText(rc_text, scale, text->Begin());
		}
	top+=line_height;
	}
BOOL show_cursor=false;
if(uCursorTime!=0)
	{
	UINT cursor_time=GetTickCount()-uCursorTime;
	show_cursor=(cursor_time%1000)<500;
	}
if(ptCursor.Left<0)
	show_cursor=false;
if(show_cursor)
	{
	POINT pt_cursor=PointFromChar(ptCursor, scale);
	POINT pt_to(pt_cursor.Left, pt_cursor.Top+line_height);
	auto brush=theme->GetTextBrush();
	target->DrawLine(pt_cursor, pt_to, brush);
	}
}

VOID Input::ReplaceSelection(LPCTSTR replace)
{
if(ReadOnly)
	return;
if(!replace)
	replace=TEXT("");
UINT line_count=cLines.get_count();
Handle<String> str_before;
Handle<String> str_after;
LPCTSTR before=TEXT("");
LPCTSTR after=TEXT("");
UINT before_len=0;
UINT after_len=0;
if(ptSelectionFirst.Top<line_count)
	{
	if(ptSelectionFirst.Left>0)
		{
		INPUT_LINE& first_line=cLines.get_at(ptSelectionFirst.Top);
		auto text=first_line.Text->Begin();
		str_before=new String(ptSelectionFirst.Left, text);
		before=str_before->Begin();
		before_len=ptSelectionFirst.Left;
		}
	}
if(ptSelectionLast.Top<line_count)
	{
	INPUT_LINE& last_line=cLines.get_at(ptSelectionLast.Top);
	UINT line_len=last_line.Offsets.get_count();
	if(ptSelectionLast.Left<line_len)
		{
		auto text=last_line.Text->Begin();
		str_after=new String(&text[ptSelectionLast.Left]);
		after=str_after->Begin();
		after_len=StringLength(after);
		}
	}
UINT line_id=ptSelectionFirst.Top;
for(auto it=cLines.begin(ptSelectionFirst.Top); it.has_current(); line_id++)
	{
	INPUT_LINE& line=it.get_current();
	UINT line_width=GetLineWidth(line, 1.f);
	if(line_width>0)
		{
		UINT& width_count=cLineWidths.get(line_width);
		if(--width_count==0)
			cLineWidths.remove(line_width);
		}
	it.remove_current();
	if(line_id==ptSelectionLast.Top)
		break;
	}
UINT line_start=0;
UINT str_pos=0;
while(replace[str_pos])
	{
	if(replace[str_pos]!='\r')
		{
		str_pos++;
		continue;
		}
	UINT insert_len=str_pos-line_start;
	UINT line_len=before_len+insert_len;
	Handle<String> text=new String(line_len, nullptr);
	auto str=const_cast<LPTSTR>(text->Begin());
	if(before)
		{
		StringCopy(str, line_len+1, before);
		StringCopy(&str[before_len], line_len+1-before_len, &replace[line_start], insert_len);
		before=nullptr;
		before_len=0;
		}
	else
		{
		StringCopy(str, line_len+1, &replace[line_start], insert_len);
		}
	INPUT_LINE& line=cLines.insert_at(ptSelectionStart.Top);
	line.Text=text;
	UpdateLine(line);
	ptSelectionFirst.Top++;
	ptSelectionFirst.Left=0;
	str_pos+=2;
	line_start=str_pos;
	}
UINT insert_len=str_pos-line_start;
UINT line_len=before_len+insert_len+after_len;
INPUT_LINE& line=cLines.insert_at(ptSelectionFirst.Top);
if(line_len>0)
	{
	Handle<String> text=new String(line_len, nullptr);
	auto str=const_cast<LPTSTR>(text->Begin());
	UINT pos=0;
	if(before_len)
		pos+=StringCopy(&str[pos], line_len+1-pos, before);
	if(insert_len)
		pos+=StringCopy(&str[pos], line_len+1-pos, &replace[line_start], insert_len);
	if(after_len)
		pos+=StringCopy(&str[pos], line_len+1-pos, after);
	line.Text=text;
	UpdateLine(line);
	}
ptSelectionFirst.Left=before_len+insert_len;
ptSelectionLast=ptSelectionFirst;
ptSelectionStart=ptSelectionFirst;
ptSelectionEnd=ptSelectionFirst;
ptCursor=ptSelectionFirst;
Invalidate(true);
SelectionChanged(this);
}

VOID Input::SelectAll()
{
POINT pt_start(0, 0);
POINT pt_end=GetEndPoint();
SetSelection(pt_end, pt_start);
}

VOID Input::SetSelection(POINT const& pt_start, POINT const& pt_end)
{
BOOL changed=false;
if(ptSelectionStart!=pt_start)
	{
	ptSelectionStart=pt_start;
	changed=true;
	}
if(ptSelectionEnd!=pt_end)
	{
	ptSelectionEnd=pt_end;
	changed=true;
	}
if(changed)
	UpdateSelection();
}

VOID Input::SetText(Handle<String> text)
{
if(!text)
	{
	Clear();
	return;
	}
Handle<StaticBuffer> buf=new StaticBuffer(text->Begin());
ReadFromStream(buf);
}


//================
// Common Private
//================

Graphics::POINT Input::CharFromPoint(POINT pt, UINT line_height)
{
UINT line_count=cLines.get_count();
if(!line_count)
	return POINT(0, 0);
if(pt.Left<0)
	pt.Left=0;
if(pt.Top<0)
	pt.Top=0;
UINT line_id=pt.Top/line_height;
if(line_id>=line_count)
	line_id=line_count-1;
if(pt.Left<0)
	return POINT(0, line_id);
INPUT_LINE& line=cLines.get_at(line_id);
FLOAT scale=GetScaleFactor();
UINT char_id=0;
UINT left=0;
for(auto it=line.Offsets.cbegin(); it.has_current(); it.move_next(), char_id++)
	{
	UINT right=it.get_current()*scale;
	if(pt.Left<right)
		{
		UINT char_width=right-left;
		UINT char_x=pt.Left-left;
		if(char_x<char_width/2)
			return POINT(char_id, line_id);
		return POINT(char_id+1, line_id);
		}
	left=right;
	}
return POINT(char_id, line_id);
}

Graphics::POINT Input::GetEndPoint()
{
POINT pt_end;
UINT line_count=cLines.get_count();
if(line_count==0)
	return pt_end;
INPUT_LINE& line=cLines.get_at(line_count-1);
UINT char_count=line.Offsets.get_count();
pt_end.Set(char_count, line_count-1);
return pt_end;
}

UINT Input::GetLineHeight(RenderTarget* target, FLOAT scale)
{
auto theme=GetTheme();
auto font=theme->GetDefaultFont();
SIZE size=target->MeasureText(font, scale, TEXT("Ag"), 2);
return size.Height;
}

UINT Input::GetLineWidth(INPUT_LINE const& line, FLOAT scale)
{
UINT count=line.Offsets.get_count();
if(!count)
	return 0;
UINT offset=line.Offsets.get_at(count-1);
return offset*scale;
}

UINT Input::GetText(POINT const& pt_start, POINT const& pt_end, LPTSTR buf, UINT size)
{
UINT line_count=cLines.get_count();
if(!line_count)
	return 0;
if(pt_start.Top==pt_end.Top)
	{
	INPUT_LINE const& line=cLines.get_at(pt_start.Top);
	UINT copy=pt_end.Left-pt_start.Left;
	auto text=line.Text->Begin();
	return StringCopy(buf, size, &text[pt_start.Left], copy);
	}
UINT line_id=pt_start.Top;
auto it=cLines.cbegin(line_id);
INPUT_LINE const& first_line=it.get_current();
auto text=first_line.Text->Begin();
UINT len=StringCopy(buf, size, &text[pt_start.Left]);
it.move_next();
for(++line_id; line_id<pt_end.Top; it.move_next(), line_id++)
	{
	len+=StringCopy(&buf[len], size-len, TEXT("\r\n"));
	INPUT_LINE const& line=it.get_current();
	text=line.Text->Begin();
	len+=StringCopy(&buf[len], size-len, text);
	}
len+=StringCopy(&buf[len], size-len, TEXT("\r\n"));
INPUT_LINE const& last_line=it.get_current();
text=last_line.Text->Begin();
len+=StringCopy(&buf[len], size-len, text, pt_end.Left);
return len;
}

UINT Input::GetTextLength(POINT const& pt_start, POINT const& pt_end)
{
UINT line_count=cLines.get_count();
if(!line_count)
	return 0;
if(pt_start.Top==pt_end.Top)
	return pt_end.Left-pt_start.Left;
UINT line_id=pt_start.Top;
auto it=cLines.cbegin(line_id);
INPUT_LINE const& first_line=it.get_current();
UINT line_len=first_line.Offsets.get_count();
UINT len=line_len-pt_start.Left;
it.move_next();
for(++line_id; line_id<pt_end.Top; it.move_next(), line_id++)
	{
	len+=2;
	INPUT_LINE const& line=it.get_current();
	len+=line.Offsets.get_count();
	}
len+=2;
len+=pt_end.Left;
return len;
}

VOID Input::MoveCursor(INT x, INT y, BOOL select, BOOL notify)
{
if(ptCursor.Left<0)
	return;
UINT line_count=cLines.get_count();
if(line_count==0)
	return;
INT line_id=ptCursor.Top+y;
line_id=MAX(line_id, 0);
line_id=MIN(line_id, line_count-1);
auto it=cLines.cbegin(line_id);
INPUT_LINE const& line=it.get_current();
UINT line_len=line.Offsets.get_count();
UINT char_id=ptCursor.Left;
if(char_id>line_len)
	char_id=line_len;
for(; x>0; x--)
	{
	if(char_id<line_len)
		{
		char_id++;
		continue;
		}
	if(line_id==line_count-1)
		break;
	if(!it.move_next())
		break;
	INPUT_LINE const& line=it.get_current();
	line_len=line.Offsets.get_count();
	line_id++;
	char_id=0;
	}
for(; x<0; x++)
	{
	if(char_id>0)
		{
		char_id--;
		continue;
		}
	if(!it.move_previous())
		break;
	INPUT_LINE const& line=it.get_current();
	line_len=line.Offsets.get_count();
	line_id--;
	char_id=line_len;
	}
POINT pt_cursor(char_id, line_id);
if(select)
	{
	ptSelectionEnd=pt_cursor;
	}
else
	{
	ptSelectionStart=pt_cursor;
	ptSelectionEnd=pt_cursor;
	}
UpdateSelection();
}

VOID Input::NotifySelectionChanged()
{
SelectionChanged(this);
}

VOID Input::OnCursorTimer()
{
Invalidate();
}

VOID Input::OnFocused()
{
ShowCursor(true);
Invalidate();
}

VOID Input::OnFocusLost()
{
ShowCursor(false);
if(GetFlag(uInputFlags, InputFlags::Pointing))
	{
	ClearFlag(uInputFlags, InputFlags::Pointing);
	ReleasePointer();
	ptPointer.Set(0, 0);
	}
}

VOID Input::OnKey(Handle<KeyEventArgs> args)
{
ShowCursor(true);
Invalidate();
args->Handled=true;
}

VOID Input::OnKeyDown(Handle<KeyEventArgs> args)
{
if(args->Char)
	{
	TCHAR str[2];
	str[0]=args->Char;
	str[1]=0;
	ReplaceSelection(str);
	OnKey(args);
	return;
	}
VirtualKey key=args->Key;
BOOL shift=GetFlag(uInputFlags, InputFlags::Shift);
switch(key)
	{
	case VirtualKey::Back:
		{
		if(!ReadOnly)
			{
			if(ptSelectionStart==ptSelectionEnd)
				MoveCursor(-1, 0, true, false);
			ReplaceSelection(nullptr);
			}
		OnKey(args);
		break;
		}
	case VirtualKey::Delete:
		{
		if(!ReadOnly)
			{
			if(ptSelectionStart==ptSelectionEnd)
				MoveCursor(1, 0, true, false);
			ReplaceSelection(nullptr);
			}
		OnKey(args);
		break;
		}
	case VirtualKey::Down:
		{
		MoveCursor(0, 1, shift);
		OnKey(args);
		break;
		}
	case VirtualKey::Escape:
		{
		ClearSelection();
		KillFocus();
		args->Handled=true;
		break;
		}
	case VirtualKey::Left:
		{
		MoveCursor(-1, 0, shift);
		OnKey(args);
		break;
		}
	case VirtualKey::Return:
		{
		if(!MultiLine)
			break;
		ReplaceSelection(TEXT("\r\n"));
		OnKey(args);
		break;
		}
	case VirtualKey::Right:
		{
		MoveCursor(1, 0, shift);
		OnKey(args);
		break;
		}
	case VirtualKey::Shift:
		{
		SetFlag(uInputFlags, InputFlags::Shift);
		args->Handled=true;
		break;
		}
	case VirtualKey::Up:
		{
		MoveCursor(0, -1, shift);
		OnKey(args);
		break;
		}
	}
}

VOID Input::OnKeyUp(Handle<KeyEventArgs> args)
{
switch(args->Key)
	{
	case VirtualKey::Shift:
		{
		ClearFlag(uInputFlags, InputFlags::Shift);
		args->Handled=true;
		break;
		}
	}
}

VOID Input::OnPointerDown(Handle<PointerEventArgs> args)
{
SetFocus(FocusReason::Pointer);
if(args->Button==PointerButton::Wheel)
	return;
if(args->Button==PointerButton::Left)
	SetFlag(uInputFlags, InputFlags::Pointing);
ptPointer=args->Point;
CapturePointer();
BOOL selection=false;
if(args->Button==PointerButton::Right)
	selection=PointIsSelected(ptPointer);
if(!selection)
	{
	ClearSelection();
	UpdatePointer();
	}
args->Handled=true;
}

VOID Input::OnPointerMoved(Handle<PointerEventArgs> args)
{
if(!GetFlag(uInputFlags, InputFlags::Pointing))
	return;
ptPointer=args->Point;
UpdatePointer();
}

VOID Input::OnPointerUp(Handle<PointerEventArgs> args)
{
if(args->Button==PointerButton::Wheel)
	return;
if(!IsCapturingPointer())
	return;
ReleasePointer();
switch(args->Button)
	{
	case PointerButton::Left:
		{
		ClearFlag(uInputFlags, InputFlags::Pointing);
		ptPointer=args->Point;
		UpdatePointer();
		break;
		}
	case PointerButton::Right:
		{
		ShowContextMenu(args->Point);
		break;
		}
	}
args->Handled=true;
}

Graphics::POINT Input::PointFromChar(POINT const& pt_char, FLOAT scale)
{
POINT pt(Padding.Left*scale, Padding.Top*scale);
UINT line_count=cLines.get_count();
if(pt_char.Top>=line_count)
	return pt;
UINT line_height=uLineHeight*scale;
pt.Top+=pt_char.Top*line_height;
if(pt_char.Left==0)
	return pt;
INPUT_LINE& line=cLines.get_at(pt_char.Top);
pt.Left+=line.Offsets.get_at(pt_char.Left-1)*scale;
return pt;
}

BOOL Input::PointIsSelected(POINT const& pt)
{
if(ptSelectionFirst==ptSelectionLast)
	return false;
FLOAT scale=GetScaleFactor();
RECT rc=GetClientRect();
rc.Left+=Padding.Left*scale;
rc.Top+=Padding.Top*scale;
UINT line_height=uLineHeight*scale;
UINT line_pt=(pt.Top-rc.Top)/line_height;
if(line_pt<ptSelectionFirst.Top||line_pt>ptSelectionLast.Top)
	return false;
POINT pt_first=PointFromChar(ptSelectionFirst, scale);
POINT pt_last=PointFromChar(ptSelectionLast, scale);
if(ptSelectionFirst.Top==ptSelectionLast.Top)
	{
	RECT rc_sel(pt_first.Left, pt_first.Top, pt_last.Left, pt_last.Top+line_height);
	return pt.Inside(rc_sel);
	}
if(line_pt==ptSelectionFirst.Top)
	{
	RECT rc_first(pt_first.Left, pt_first.Top, rc.Right, pt_first.Top+line_height);
	return pt.Inside(rc_first);
	}
if(line_pt==ptSelectionLast.Top)
	{
	RECT rc_last(rc.Left, pt_last.Top, pt_last.Left, pt_last.Top+line_height);
	return pt.Inside(rc_last);
	}
return true;
}

BOOL Input::ShowContextMenu(POINT pt)
{
if(!ContextMenu)
	return false;
auto edit_menu=Convert<EditMenu>(ContextMenu);
if(edit_menu)
	{
	auto clipboard=Clipboard::Open();
	BOOL content=cLines.get_count()>0;
	BOOL paste=clipboard->HasText();
	BOOL selection=ptSelectionFirst!=ptSelectionLast;
	if(!content&&!paste)
		return false;
	edit_menu->SelectAll->Visible=(content&&!selection);
	edit_menu->Copy->Visible=selection;
	edit_menu->Cut->Visible=(!ReadOnly&&selection);
	edit_menu->Delete->Visible=(!ReadOnly&&selection);
	edit_menu->Paste->Visible=(!ReadOnly&&paste);
	}
POINT pt_frame=GetFrameOffset();
pt+=pt_frame;
ContextMenu->Show(this, pt);
return true;
}

VOID Input::ShowCursor(BOOL show)
{
uCursorTime=(show? GetTickCount(): 0);
Invalidate();
if(show)
	{
	if(hCursorTimer)
		{
		hCursorTimer->Reset();
		}
	else
		{
		hCursorTimer=new Timer();
		hCursorTimer->Triggered.Add(this, &Input::OnCursorTimer);
		hCursorTimer->StartPeriodic(500);
		}
	}
else
	{
	hCursorTimer=nullptr;
	}
}

VOID Input::UpdateLine(INPUT_LINE& line)
{
auto target=GetTarget();
auto font=GetFont();
UINT char_count=line.Offsets.get_count();
if(char_count>0)
	{
	UINT line_width=line.Offsets.get_at(char_count-1);
	UINT& width_count=cLineWidths.get(line_width);
	if(--width_count==0)
		cLineWidths.remove(line_width);
	line.Offsets.clear();
	char_count=0;
	}
if(!line.Text)
	return;
auto str=line.Text->Begin();
SIZE size;
for(UINT pos=0; str[pos]; pos++)
	{
	size=target->MeasureText(font, 1.0, str, pos+1);
	line.Offsets.append(size.Width);
	}
UINT line_width=size.Width;
UINT& width_count=cLineWidths.get(line_width, 0);
width_count++;
}

VOID Input::UpdatePointer()
{
FLOAT scale=GetScaleFactor();
UINT left=Padding.Left*scale;
UINT top=Padding.Top*scale;
POINT pt(ptPointer.Left-left, ptPointer.Top-top);
UINT line_height=uLineHeight*scale;
pt=CharFromPoint(pt, line_height);
BOOL changed=false;
if(ptSelectionStart.Left<0)
	{
	ptSelectionStart=pt;
	ptSelectionEnd=pt;
	changed=true;
	}
else
	{
	if(ptSelectionEnd!=pt)
		{
		ptSelectionEnd=pt;
		changed=true;
		}
	}
UpdateSelection();
}

VOID Input::UpdateSelection()
{
UINT start_line=MIN(ptSelectionStart.Top, ptSelectionEnd.Top);
UINT end_line=MAX(ptSelectionStart.Top, ptSelectionEnd.Top);
UINT start_char=0;
UINT end_char=0;
BOOL reverse=false;
if(start_line==end_line)
	{
	reverse=ptSelectionStart.Left>ptSelectionEnd.Left;
	start_char=MIN(ptSelectionStart.Left, ptSelectionEnd.Left);
	end_char=MAX(ptSelectionStart.Left, ptSelectionEnd.Left);
	}
else if(start_line==ptSelectionStart.Top)
	{
	start_char=ptSelectionStart.Left;
	end_char=ptSelectionEnd.Left;
	}
else
	{
	reverse=true;
	start_char=ptSelectionEnd.Left;
	end_char=ptSelectionStart.Left;
	}
ptSelectionFirst.Set(start_char, start_line);
ptSelectionLast.Set(end_char, end_line);
if(reverse)
	{
	ptCursor.Set(start_char, start_line);
	}
else
	{
	ptCursor.Set(end_char, end_line);
	}
Invalidate(true);
auto app=Application::Current;
app->Dispatch(this, &Input::NotifySelectionChanged);
}

}}