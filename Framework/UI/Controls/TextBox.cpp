//=============
// TextBox.cpp
//=============

#include "TextBox.h"


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

Handle<Brush> TextBox::GetBackground()
{
return m_Theme->WindowBrush;
}

VOID TextBox::ReadFromStream(IInputStream* stream)
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


//==========================
// Con-/Destructors Private
//==========================

TextBox::TextBox(Window* parent):
ScrollBox(parent),
ReadOnly(this)
{
AlignHorizontal=HorizontalAlignment::Stretch;
AlignVertical=VerticalAlignment::Stretch;
Enabled.Changed.Add(this, &TextBox::OnEnabledChanged);
ReadOnly.Changed.Add(this, &TextBox::OnReadOnlyChanged);
ZoomMax=10.f;
m_Input=Input::Create(Body);
m_Input->MultiLine=true;
m_Input->Padding.Set(2, 2, 2, 2);
m_Input->SelectionChanged.Add(this, &TextBox::OnInputSelectionChanged);
}


//================
// Common Private
//================

VOID TextBox::OnEnabledChanged(BOOL enabled)
{
m_Input->Enabled=enabled;
}

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