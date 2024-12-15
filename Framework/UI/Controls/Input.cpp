//===========
// Input.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Concurrency/MainTask.h"
#include "Storage/Clipboard.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/StaticBuffer.h"
#include "UI/Controls/Menus/EditMenu.h"
#include "UI/Application.h"
#include "Input.h"

using namespace Concurrency;
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
m_CursorPos(-1, -1),
m_CursorTime(0),
m_InputFlags(InputFlags::None),
m_LineHeight(0),
m_PointerPos(-1, -1),
m_SelectionEnd(-1, -1),
m_SelectionFirst(-1, -1),
m_SelectionLast(-1, -1),
m_SelectionStart(-1, -1)
{
ContextMenu=new EditMenu();
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

VOID Input::AppendLine(Handle<String> str)
{
INPUT_LINE& line=m_Lines.append();
line.Text=str;
UpdateLine(line);
Invalidate(true);
}

VOID Input::AppendLines(Handle<StringList> lines)
{
if(!lines)
	return;
for(auto it=lines->First(); it->HasCurrent(); it->MoveNext())
	{
	INPUT_LINE& line=m_Lines.append();
	line.Text=it->GetCurrent();
	UpdateLine(line);
	}
Invalidate(true);
}

VOID Input::Clear()
{
ClearSelection();
m_Lines.clear();
m_LineWidths.clear();
}

VOID Input::ClearSelection()
{
BOOL changed=m_SelectionStart.Left>=0;
m_SelectionStart.Set(-1, -1);
m_SelectionEnd.Set(-1, -1);
if(changed)
	UpdateSelection();
}

Handle<Brush> Input::GetBackgroundBrush()
{
auto theme=GetTheme();
if(!IsEnabled())
	return theme->ControlBrush;
return theme->WindowBrush;
}

Handle<Cursor> Input::GetCursor()
{
auto theme=GetTheme();
return theme->TextCursor;
}

Graphics::RECT Input::GetCursorRect()
{
FLOAT scale=GetScaleFactor();
POINT pt_cursor=PointFromChar(m_CursorPos, scale);
if(GetFlag(m_InputFlags, InputFlags::Pointing))
	pt_cursor=m_PointerPos;
UINT line_height=m_LineHeight*scale;
RECT rc_cursor(pt_cursor.Left, pt_cursor.Top, pt_cursor.Left, pt_cursor.Top+line_height);
rc_cursor.SetMargin(Padding*scale);
return rc_cursor;
}

UINT Input::GetLineHeight()const
{
FLOAT scale=GetScaleFactor();
return m_LineHeight*scale;
}

Graphics::SIZE Input::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
SIZE size(0, 0);
UINT width_count=m_LineWidths.get_count();
if(width_count>0)
	{
	size.Width=m_LineWidths.get_at(width_count-1).get_key();
	size.Width*=scale;
	}
m_LineHeight=GetLineHeight(target, 1.f);
UINT line_count=Max(m_Lines.get_count(), 1U);
UINT line_height=m_LineHeight*scale;
size.Height=line_count*line_height;
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

Handle<String> Input::GetSelection()
{
UINT len=GetTextLength(m_SelectionFirst, m_SelectionLast);
if(!len)
	return nullptr;
Handle<String> text=new String(len, nullptr);
auto buf=const_cast<LPTSTR>(text->Begin());
GetText(m_SelectionFirst, m_SelectionLast, buf, len+1);
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

BOOL Input::KillFocus()
{
auto app=Application::Current;
if(app->GetCurrentInput()==this)
	app->SetCurrentInput(nullptr);
return Interactive::KillFocus();
}

VOID Input::ReadFromStream(InputStream* stream)
{
Clear();
StreamReader reader(stream);
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
target->TextColor=theme->TextBrush;
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
UINT client_height=rc.GetHeight();
POINT offset=target->GetOffset();
UINT line_height=m_LineHeight*scale;
UINT first_line=offset.Top/line_height;
UINT line_count=client_height/line_height+2;
UINT last_line=first_line+line_count-1;
BOOL show_sel=true;
if(m_SelectionFirst==m_SelectionLast)
	show_sel=false;
if(Application::Current->GetCurrentInput()!=this)
	show_sel=false;
if(show_sel)
	{
	auto highlight=theme->HighlightBrush;
	if(!HasFocus())
		highlight=theme->InactiveHighlightBrush;
	POINT pt_first=PointFromChar(m_SelectionFirst, scale);
	POINT pt_last=PointFromChar(m_SelectionLast, scale);
	if(m_SelectionFirst.Top==m_SelectionLast.Top)
		{
		RECT rc_fill(pt_first.Left, pt_first.Top, pt_last.Left, pt_last.Top+line_height);
		target->FillRect(rc_fill, highlight);
		}
	else
		{
		auto it=m_Lines.cbegin(m_SelectionFirst.Top);
		UINT right=rc.Left+GetLineWidth(it.get_current(), scale);
		RECT rc_first(pt_first.Left, pt_first.Top, right, pt_first.Top+line_height);
		target->FillRect(rc_first, highlight);
		for(UINT line=m_SelectionFirst.Top+1; line<m_SelectionLast.Top; line++)
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
for(auto it=m_Lines.cbegin(first_line); it.has_current(); it.move_next(), line++)
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
if(m_CursorTime!=0)
	{
	UINT cursor_time=GetTickCount()-m_CursorTime;
	show_cursor=(cursor_time%1000)<500;
	}
if(m_CursorPos.Left<0)
	show_cursor=false;
if(show_cursor)
	{
	POINT pt_cursor=PointFromChar(m_CursorPos, scale);
	POINT pt_to(pt_cursor.Left, pt_cursor.Top+line_height);
	auto brush=theme->TextBrush;
	target->DrawLine(pt_cursor, pt_to, brush);
	}
}

VOID Input::ReplaceSelection(LPCTSTR replace)
{
if(ReadOnly)
	return;
if(!replace)
	replace=TEXT("");
UINT line_count=m_Lines.get_count();
Handle<String> str_before;
Handle<String> str_after;
LPCTSTR before=TEXT("");
LPCTSTR after=TEXT("");
UINT before_len=0;
UINT after_len=0;
if(m_SelectionFirst.Top<line_count)
	{
	if(m_SelectionFirst.Left>0)
		{
		INPUT_LINE& first_line=m_Lines.get_at(m_SelectionFirst.Top);
		auto text=first_line.Text->Begin();
		str_before=new String(m_SelectionFirst.Left, text);
		before=str_before->Begin();
		before_len=m_SelectionFirst.Left;
		}
	}
if(m_SelectionLast.Top<line_count)
	{
	INPUT_LINE& last_line=m_Lines.get_at(m_SelectionLast.Top);
	UINT line_len=last_line.Offsets.get_count();
	if(m_SelectionLast.Left<line_len)
		{
		auto text=last_line.Text->Begin();
		str_after=new String(&text[m_SelectionLast.Left]);
		after=str_after->Begin();
		after_len=StringHelper::Length(after);
		}
	}
UINT line_id=m_SelectionFirst.Top;
for(auto it=m_Lines.begin(m_SelectionFirst.Top); it.has_current(); line_id++)
	{
	INPUT_LINE& line=it.get_current();
	UINT line_width=GetLineWidth(line, 1.f);
	if(line_width>0)
		{
		UINT& width_count=m_LineWidths.get(line_width);
		if(--width_count==0)
			m_LineWidths.remove(line_width);
		}
	it.remove_current();
	if(line_id==m_SelectionLast.Top)
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
		StringHelper::Copy(str, line_len+1, before);
		StringHelper::Copy(&str[before_len], line_len+1-before_len, &replace[line_start], insert_len);
		before=nullptr;
		before_len=0;
		}
	else
		{
		StringHelper::Copy(str, line_len+1, &replace[line_start], insert_len);
		}
	INPUT_LINE& line=m_Lines.insert_at(m_SelectionStart.Top);
	line.Text=text;
	UpdateLine(line);
	m_SelectionFirst.Top++;
	m_SelectionFirst.Left=0;
	str_pos+=2;
	line_start=str_pos;
	}
UINT insert_len=str_pos-line_start;
UINT line_len=before_len+insert_len+after_len;
INPUT_LINE& line=m_Lines.insert_at(m_SelectionFirst.Top);
if(line_len>0)
	{
	Handle<String> text=new String(line_len, nullptr);
	auto str=const_cast<LPTSTR>(text->Begin());
	UINT pos=0;
	if(before_len)
		pos+=StringHelper::Copy(&str[pos], line_len+1-pos, before);
	if(insert_len)
		pos+=StringHelper::Copy(&str[pos], line_len+1-pos, &replace[line_start], insert_len);
	if(after_len)
		pos+=StringHelper::Copy(&str[pos], line_len+1-pos, after);
	line.Text=text;
	UpdateLine(line);
	}
m_SelectionFirst.Left=before_len+insert_len;
m_SelectionLast=m_SelectionFirst;
m_SelectionStart=m_SelectionFirst;
m_SelectionEnd=m_SelectionFirst;
m_CursorPos=m_SelectionFirst;
Invalidate(true);
SelectionChanged(this);
}

VOID Input::SelectAll()
{
POINT pt_start(0, 0);
POINT pt_end=GetEndPoint();
SetSelection(pt_start, pt_end);
}

VOID Input::SelectNone()
{
SetSelection(m_SelectionEnd, m_SelectionEnd);
}

VOID Input::SetFocus(FocusReason reason)
{
Interactive::SetFocus(reason);
Application::Current->SetCurrentInput(this);
}

VOID Input::SetSelection(POINT const& pt_start, POINT const& pt_end)
{
BOOL changed=false;
if(m_SelectionStart!=pt_start)
	{
	m_SelectionStart=pt_start;
	changed=true;
	}
if(m_SelectionEnd!=pt_end)
	{
	m_SelectionEnd=pt_end;
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
UINT line_count=m_Lines.get_count();
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
INPUT_LINE& line=m_Lines.get_at(line_id);
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
UINT line_count=m_Lines.get_count();
if(line_count==0)
	return pt_end;
INPUT_LINE& line=m_Lines.get_at(line_count-1);
UINT char_count=line.Offsets.get_count();
pt_end.Set(char_count, line_count-1);
return pt_end;
}

UINT Input::GetLineHeight(RenderTarget* target, FLOAT scale)
{
auto theme=GetTheme();
auto font=theme->DefaultFont;
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
UINT line_count=m_Lines.get_count();
if(!line_count)
	return 0;
if(pt_start.Top==pt_end.Top)
	{
	INPUT_LINE const& line=m_Lines.get_at(pt_start.Top);
	UINT copy=pt_end.Left-pt_start.Left;
	auto text=line.Text->Begin();
	return StringHelper::Copy(buf, size, &text[pt_start.Left], copy);
	}
UINT line_id=pt_start.Top;
auto it=m_Lines.cbegin(line_id);
INPUT_LINE const& first_line=it.get_current();
auto text=first_line.Text->Begin();
UINT len=StringHelper::Copy(buf, size, &text[pt_start.Left]);
it.move_next();
for(++line_id; line_id<pt_end.Top; it.move_next(), line_id++)
	{
	len+=StringHelper::Copy(&buf[len], size-len, TEXT("\r\n"));
	INPUT_LINE const& line=it.get_current();
	text=line.Text->Begin();
	len+=StringHelper::Copy(&buf[len], size-len, text);
	}
len+=StringHelper::Copy(&buf[len], size-len, TEXT("\r\n"));
INPUT_LINE const& last_line=it.get_current();
text=last_line.Text->Begin();
len+=StringHelper::Copy(&buf[len], size-len, text, pt_end.Left);
return len;
}

UINT Input::GetTextLength(POINT const& pt_start, POINT const& pt_end)
{
UINT line_count=m_Lines.get_count();
if(!line_count)
	return 0;
if(pt_start.Top==pt_end.Top)
	return pt_end.Left-pt_start.Left;
UINT line_id=pt_start.Top;
auto it=m_Lines.cbegin(line_id);
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
if(m_CursorPos.Left<0)
	return;
UINT line_count=m_Lines.get_count();
if(line_count==0)
	return;
INT line_id=m_CursorPos.Top+y;
line_id=Max(line_id, 0);
line_id=Min(line_id, (INT)line_count-1);
auto it=m_Lines.cbegin(line_id);
INPUT_LINE const& line=it.get_current();
UINT line_len=line.Offsets.get_count();
UINT char_id=m_CursorPos.Left;
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
	m_SelectionEnd=pt_cursor;
	}
else
	{
	m_SelectionStart=pt_cursor;
	m_SelectionEnd=pt_cursor;
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
if(GetFlag(m_InputFlags, InputFlags::Pointing))
	{
	ClearFlag(m_InputFlags, InputFlags::Pointing);
	ReleasePointer();
	m_PointerPos.Set(0, 0);
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
BOOL shift=GetFlag(m_InputFlags, InputFlags::Shift);
switch(key)
	{
	case VirtualKey::Back:
		{
		if(!ReadOnly)
			{
			if(m_SelectionStart==m_SelectionEnd)
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
			if(m_SelectionStart==m_SelectionEnd)
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
		SetFlag(m_InputFlags, InputFlags::Shift);
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
		ClearFlag(m_InputFlags, InputFlags::Shift);
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
	SetFlag(m_InputFlags, InputFlags::Pointing);
m_PointerPos=args->Point;
CapturePointer();
BOOL selection=false;
if(args->Button==PointerButton::Right)
	selection=PointIsSelected(m_PointerPos);
if(!selection)
	{
	ClearSelection();
	UpdatePointer();
	}
args->Handled=true;
}

VOID Input::OnPointerMoved(Handle<PointerEventArgs> args)
{
if(!GetFlag(m_InputFlags, InputFlags::Pointing))
	return;
m_PointerPos=args->Point;
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
		ClearFlag(m_InputFlags, InputFlags::Pointing);
		m_PointerPos=args->Point;
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
UINT line_count=m_Lines.get_count();
if(pt_char.Top>=line_count)
	return pt;
UINT line_height=m_LineHeight*scale;
pt.Top+=pt_char.Top*line_height;
if(pt_char.Left==0)
	return pt;
INPUT_LINE& line=m_Lines.get_at(pt_char.Top);
pt.Left+=line.Offsets.get_at(pt_char.Left-1)*scale;
return pt;
}

BOOL Input::PointIsSelected(POINT const& pt)
{
if(m_SelectionFirst==m_SelectionLast)
	return false;
FLOAT scale=GetScaleFactor();
RECT rc=GetClientRect();
rc.Left+=Padding.Left*scale;
rc.Top+=Padding.Top*scale;
UINT line_height=m_LineHeight*scale;
UINT line_pt=(pt.Top-rc.Top)/line_height;
if(line_pt<m_SelectionFirst.Top||line_pt>m_SelectionLast.Top)
	return false;
POINT pt_first=PointFromChar(m_SelectionFirst, scale);
POINT pt_last=PointFromChar(m_SelectionLast, scale);
if(m_SelectionFirst.Top==m_SelectionLast.Top)
	{
	RECT rc_sel(pt_first.Left, pt_first.Top, pt_last.Left, pt_last.Top+line_height);
	return pt.Inside(rc_sel);
	}
if(line_pt==m_SelectionFirst.Top)
	{
	RECT rc_first(pt_first.Left, pt_first.Top, rc.Right, pt_first.Top+line_height);
	return pt.Inside(rc_first);
	}
if(line_pt==m_SelectionLast.Top)
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
	BOOL content=m_Lines.get_count()>0;
	BOOL paste=clipboard->HasText();
	BOOL selection=m_SelectionFirst!=m_SelectionLast;
	if(!content&&!paste)
		return false;
	edit_menu->SelectAll->Visible=(content&&!selection);
	edit_menu->Copy->Visible=selection;
	edit_menu->Cut->Visible=(!ReadOnly&&selection);
	edit_menu->Delete->Visible=(!ReadOnly&&selection);
	edit_menu->Paste->Visible=(!ReadOnly&&paste);
	}
pt+=GetScreenOffset();
ContextMenu->Show(pt);
return true;
}

VOID Input::ShowCursor(BOOL show)
{
m_CursorTime=(show? GetTickCount(): 0);
Invalidate();
if(show)
	{
	if(m_CursorTimer)
		{
		m_CursorTimer->Reset();
		}
	else
		{
		m_CursorTimer=new Timer();
		m_CursorTimer->Triggered.Add(this, &Input::OnCursorTimer);
		m_CursorTimer->StartPeriodic(500);
		}
	}
else
	{
	m_CursorTimer=nullptr;
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
	UINT& width_count=m_LineWidths.get(line_width);
	if(--width_count==0)
		m_LineWidths.remove(line_width);
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
UINT& width_count=m_LineWidths.get(line_width, 0);
width_count++;
}

VOID Input::UpdatePointer()
{
FLOAT scale=GetScaleFactor();
UINT left=Padding.Left*scale;
UINT top=Padding.Top*scale;
POINT pt(m_PointerPos.Left-left, m_PointerPos.Top-top);
UINT line_height=m_LineHeight*scale;
pt=CharFromPoint(pt, line_height);
BOOL changed=false;
if(m_SelectionStart.Left<0)
	{
	m_SelectionStart=pt;
	m_SelectionEnd=pt;
	changed=true;
	}
else
	{
	if(m_SelectionEnd!=pt)
		{
		m_SelectionEnd=pt;
		changed=true;
		}
	}
UpdateSelection();
}

VOID Input::UpdateSelection()
{
UINT start_line=Min(m_SelectionStart.Top, m_SelectionEnd.Top);
UINT end_line=Max(m_SelectionStart.Top, m_SelectionEnd.Top);
UINT start_char=0;
UINT end_char=0;
BOOL reverse=false;
if(start_line==end_line)
	{
	reverse=m_SelectionStart.Left>m_SelectionEnd.Left;
	start_char=Min(m_SelectionStart.Left, m_SelectionEnd.Left);
	end_char=Max(m_SelectionStart.Left, m_SelectionEnd.Left);
	}
else if(start_line==m_SelectionStart.Top)
	{
	start_char=m_SelectionStart.Left;
	end_char=m_SelectionEnd.Left;
	}
else
	{
	reverse=true;
	start_char=m_SelectionEnd.Left;
	end_char=m_SelectionStart.Left;
	}
m_SelectionFirst.Set(start_char, start_line);
m_SelectionLast.Set(end_char, end_line);
if(reverse)
	{
	m_CursorPos.Set(start_char, start_line);
	}
else
	{
	m_CursorPos.Set(end_char, end_line);
	}
Invalidate(true);
MainTask::Dispatch(this, &Input::NotifySelectionChanged);
}

}}
