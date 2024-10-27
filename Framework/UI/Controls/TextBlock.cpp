//===============
// TextBlock.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "TextBlock.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TextBlock::TextBlock(Handle<String> text):
TextBlock(nullptr, text)
{}

TextBlock::TextBlock(Window* parent, Handle<String> text):
Control(parent),
Text(this)
{
Text.Changed.Add(this, &TextBlock::OnTextChanged);
Text=text;
}


//========
// Common
//========

Graphics::SIZE TextBlock::GetMinSize(RenderTarget* target)
{
auto font=GetFont();
FLOAT scale=GetScaleFactor();
SIZE size(0, 0);
if(Text)
	{
	size=target->MeasureText(font, scale, Text->Begin());
	}
else
	{
	size.Height=font->GetSize()*scale;
	}
return size.Max(MinSize*scale);
}

VOID TextBlock::Render(RenderTarget* target, RECT& rc)
{
Control::Render(target, rc);
if(!Text)
	return;
auto text_color=TextColor;
if(!text_color)
	{
	auto theme=GetTheme();
	text_color=theme->TextBrush;
	}
target->Font=GetFont();
target->TextColor=text_color;
FLOAT scale=GetScaleFactor();
target->DrawText(rc, scale, Text->Begin());
}


//================
// Common Private
//================

VOID TextBlock::OnTextChanged(Handle<String> text)
{
Invalidate(true);
}

}}