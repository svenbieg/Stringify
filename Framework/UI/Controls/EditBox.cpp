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


//==================
// Con-/Destructors
//==================

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
hInput=new Input(Body);
hInput->Focused.Add(this, &EditBox::OnInputFocused);
hInput->Padding.Set(2, 2, 2, 2);
hInput->SelectionChanged.Add(this, &EditBox::OnInputSelectionChanged);
hInput->TabStop=true;
}


//========
// Common
//========

Graphics::SIZE EditBox::GetMinSize(RenderTarget* target)
{
SIZE size(1, 1);
SIZE min_size=hInput->GetMinSize(target);
size.Height+=min_size.Height;
FLOAT scale=GetScaleFactor();
return size.Max(MinSize*scale);
}

VOID EditBox::Rearrange(RenderTarget* target, RECT& rc)
{
UINT line_height=hInput->GetLineHeight();
HorizontalBar->Step=line_height;
ScrollBox::Rearrange(target, rc);
}


//================
// Common Private
//================

VOID EditBox::OnInputFocused(FocusReason reason)
{
if(reason==FocusReason::Keyboard)
	hInput->SelectAll();
}

VOID EditBox::OnInputSelectionChanged()
{
RECT rc_cursor=hInput->GetCursorRect();
SetHotspot(rc_cursor);
SelectionChanged(this);
}

VOID EditBox::OnMaskChanged(TCHAR mask)
{
hInput->Mask=mask;
hInput->Invalidate(true);
}

VOID EditBox::OnTextChanged(Handle<String> text)
{
hInput->SetText(text);
}

}}