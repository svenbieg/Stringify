//=============
// TextBox.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "TextBox.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TextBox::TextBox(Window* parent):
ScrollBox(parent),
ReadOnly(this)
{
AlignHorizontal=HorizontalAlignment::Stretch;
AlignVertical=VerticalAlignment::Stretch;
ReadOnly.Changed.Add(this, &TextBox::OnReadOnlyChanged);
ZoomMax=10.f;
m_Input=new Input(Body);
m_Input->MultiLine=true;
m_Input->Padding.Set(2, 2, 2, 2);
m_Input->SelectionChanged.Add(this, &TextBox::OnInputSelectionChanged);
}


//========
// Common
//========

VOID TextBox::AppendLine(Handle<String> line)
{
m_Input->AppendLine(line);
}

VOID TextBox::AppendLines(Handle<StringList> lines)
{
m_Input->AppendLines(lines);
}

VOID TextBox::Clear()
{
m_Input->Clear();
SetPosition(0, 0);
}

VOID TextBox::ReadFromStream(Handle<InputStream> stream)
{
m_Input->ReadFromStream(stream);
}

VOID TextBox::Rearrange(RenderTarget* target, RECT& rc)
{
RECT rc_input=m_Input->GetRect();
SIZE size=m_Input->GetMinSize(target);
rc_input.SetSize(size);
m_Input->Move(target, rc_input);
UINT line_height=m_Input->GetLineHeight();
HorizontalBar->Step=line_height;
VerticalBar->Step=line_height;
if(m_Hotspot)
	m_Hotspot=m_Input->GetCursorRect();
ScrollBox::Rearrange(target, rc);
}

VOID TextBox::SelectAll()
{
m_Input->SelectAll();
}

VOID TextBox::SetFocus()
{
m_Input->SetFocus();
}

VOID TextBox::SetText(Handle<String> text)
{
m_Input->SetText(text);
}


//================
// Common Private
//================

VOID TextBox::OnInputSelectionChanged()
{
RECT rc_cursor=m_Input->GetCursorRect();
SetHotspot(rc_cursor);
SelectionChanged(this);
}

VOID TextBox::OnReadOnlyChanged(BOOL read_only)
{
m_Input->ReadOnly=read_only;
}

}}