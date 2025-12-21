//===============
// TextBlock.cpp
//===============

#include "UI/Controls/TextBlock.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Graphics::SIZE TextBlock::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
SIZE size(0, 0);
auto font=m_Theme->DefaultFont;
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
auto font=m_Theme->DefaultFont;
auto brush=m_Theme->TextBrush;
FLOAT scale=GetScaleFactor();
target->DrawText(rc, scale, font, brush, Text->Begin());
}


//============================
// Con-/Destructors Protected
//============================

TextBlock::TextBlock(Window* parent, Handle<String> text):
Control(parent),
Text(this)
{
Text.Changed.Add(this, &TextBlock::OnTextChanged);
Text=text;
}


//================
// Common Private
//================

VOID TextBlock::OnTextChanged(Handle<String> text)
{
Invalidate(true);
}

}}