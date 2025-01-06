//=============
// EditBox.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "UI/Frame.h"
#include "EditBox.h"

using namespace Graphics;


//========
// Common
//========

namespace UI {
	namespace Controls {


//========
// Common
//========

Graphics::SIZE EditBox::GetMinSize(RenderTarget* target)
{
SIZE size(1, 1);
SIZE min_size=m_Input->GetMinSize(target);
size.Height+=min_size.Height;
FLOAT scale=GetScaleFactor();
return size.Max(MinSize*scale);
}

VOID EditBox::Rearrange(RenderTarget* target, RECT& rc)
{
UINT line_height=m_Input->GetLineHeight();
HorizontalBar->Step=line_height;
ScrollBox::Rearrange(target, rc);
}


//==========================
// Con-/Destructors Private
//==========================

EditBox::EditBox(Window* parent):
ScrollBox(parent),
Mask(this, 0),
Text(this)
{
AlignHorizontal=HorizontalAlignment::Stretch;
AlignVertical=VerticalAlignment::Center;
HorizontalBar->Visibility=ScrollBarVisibility::Hidden;
Mask.Changed.Add(this, &EditBox::OnMaskChanged);
Text.Changed.Add(this, &EditBox::OnTextChanged);
VerticalBar->Visibility=ScrollBarVisibility::Hidden;
m_Input=Input::Create(Body);
m_Input->Focused.Add(this, &EditBox::OnInputFocused);
m_Input->Padding.Set(2, 2, 2, 2);
m_Input->SelectionChanged.Add(this, &EditBox::OnInputSelectionChanged);
m_Input->TabStop=true;
}


//================
// Common Private
//================

VOID EditBox::OnInputFocused(FocusReason reason)
{
if(reason==FocusReason::Keyboard)
	m_Input->SelectAll();
}

VOID EditBox::OnInputSelectionChanged()
{
RECT rc_cursor=m_Input->GetCursorRect();
SetHotspot(rc_cursor);
SelectionChanged(this);
}

VOID EditBox::OnMaskChanged(TCHAR mask)
{
m_Input->Mask=mask;
m_Input->Invalidate(true);
}

VOID EditBox::OnTextChanged(Handle<String> text)
{
m_Input->SetText(text);
}

}}