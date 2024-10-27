//============
// Button.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "Button.h"
#include "Grid.h"
#include "TextBlock.h"

using namespace Core;
using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

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


//========
// Common
//========

Handle<Brush> Button::GetBackgroundBrush()
{
auto theme=GetTheme();
auto background=theme->ControlBrush;
BOOL focus=HasFocus();
focus|=HasPointerFocus();
if(focus)
	background=theme->HighlightBrush;
return background;
}

Graphics::SIZE Button::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
if(Border)
	size.AddPadding(1, 1, 1, 1);
FLOAT scale=GetScaleFactor();
if(Text)
	{
	auto font=GetFont();
	SIZE text_size=target->MeasureText(font, scale, Text->Begin());
	size.Width+=text_size.Width;
	size.Height=Max(size.Height, text_size.Height);
	}
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID Button::Render(RenderTarget* target, RECT& rc)
{
Interactive::Render(target, rc);
auto theme=GetTheme();
if(Border)
	{
	auto brush=theme->BorderBrush;
	target->DrawRect(rc, brush);
	rc.SetPadding(1, 1, 1, 1);
	}
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
if(Text)
	{
	target->TextColor=theme->TextBrush;
	target->Font=GetFont();
	target->DrawText(rc, scale, Text->Begin());
	}
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
Application::Current->Dispatch(this, &Button::DoClick);
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