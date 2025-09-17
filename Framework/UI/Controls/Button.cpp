//============
// Button.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "Button.h"
#include "Grid.h"
#include "TextBlock.h"

using namespace Concurrency;
using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Handle<Brush> Button::GetBackground()
{
return m_Theme->ControlBrush;
}

Graphics::SIZE Button::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
if(Border)
	size.AddPadding(1, 1, 1, 1);
FLOAT scale=GetScaleFactor();
if(Text)
	{
	auto font=m_Theme->DefaultFont;
	SIZE text_size=target->MeasureText(font, scale, Text->Begin());
	size.Width+=text_size.Width;
	size.Height=TypeHelper::Max(size.Height, text_size.Height);
	}
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID Button::Render(RenderTarget* target, RECT& rc)
{
Interactive::Render(target, rc);
if(Border)
	{
	auto brush=m_Theme->BorderBrush;
	target->DrawRect(rc, brush);
	rc.SetPadding(1, 1, 1, 1);
	}
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
if(Text)
	{
	auto font=m_Theme->DefaultFont;
	auto brush=m_Theme->TextBrush;
	target->DrawText(rc, scale, font, brush, Text->Begin());
	}
}


//==========================
// Con-/Destructors Private
//==========================

Button::Button(Window* parent, Handle<String> text):
Interactive(parent),
Border(false),
Padding(20, 4, 20, 4),
Text(text)
{
Interactive::Clicked.Add(this, &Button::OnInteractiveClicked);
Focused.Add(this, &Button::OnFocused);
FocusLost.Add(this, &Button::OnFocusLost);
PointerEntered.Add(this, &Button::OnPointerEntered);
PointerLeft.Add(this, &Button::OnPointerLeft);
}


//================
// Common Private
//================

VOID Button::DoClick()
{
Clicked(this);
}

VOID Button::OnFocused()
{
Invalidate();
}

VOID Button::OnFocusLost()
{
Invalidate();
}

VOID Button::OnInteractiveClicked()
{
DispatchedQueue::Append(this, &Button::DoClick);
}

VOID Button::OnPointerEntered()
{
Invalidate();
}

VOID Button::OnPointerLeft()
{
Invalidate();
}

}}