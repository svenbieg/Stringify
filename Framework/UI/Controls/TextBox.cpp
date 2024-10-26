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
hInput=new Input(Body);
hInput->MultiLine=true;
hInput->Padding.Set(2, 2, 2, 2);
hInput->SelectionChanged.Add(this, &TextBox::OnInputSelectionChanged);
}


//========
// Common
//========

VOID TextBox::AppendLine(Handle<String> text)
{
hInput->AppendLine(text);
}

VOID TextBox::Clear()
{
hInput->Clear();
SetPosition(0, 0);
}

VOID TextBox::ReadFromStream(Handle<InputStream> stream)
{
hInput->ReadFromStream(stream);
}

VOID TextBox::Rearrange(RenderTarget* target, RECT& rc)
{
RECT rc_input=hInput->GetRect();
SIZE size=hInput->GetMinSize(target);
rc_input.SetSize(size);
hInput->Move(target, rc_input);
UINT line_height=hInput->GetLineHeight();
HorizontalBar->Step=line_height;
VerticalBar->Step=line_height;
if(m_Hotspot)
	m_Hotspot=hInput->GetCursorRect();
ScrollBox::Rearrange(target, rc);
}

VOID TextBox::SelectAll()
{
hInput->SelectAll();
}

VOID TextBox::SetFocus()
{
hInput->SetFocus();
}

VOID TextBox::SetText(Handle<String> text)
{
hInput->SetText(text);
}


//================
// Common Private
//================

VOID TextBox::OnInputSelectionChanged()
{
RECT rc_cursor=hInput->GetCursorRect();
SetHotspot(rc_cursor);
SelectionChanged(this);
}

VOID TextBox::OnReadOnlyChanged(BOOL read_only)
{
hInput->ReadOnly=read_only;
}

}}