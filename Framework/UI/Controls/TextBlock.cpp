//===============
// TextBlock.cpp
//===============

#include "TextBlock.h"


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
FLOAT scale=GetScaleFactor();
target->DrawText(rc, scale, font, Color, Text->Begin());
}


//============================
// Con-/Destructors Protected
//============================

TextBlock::TextBlock(Window* parent, Handle<String> text):
Control(parent),
Color(this, m_Theme->TextBrush),
Font(this, m_Theme->DefaultFont),
Text(this, text)
{
Color.Changed.Add(this, &TextBlock::OnColorChanged);
Font.Changed.Add(this, &TextBlock::OnFontChanged);
Text.Changed.Add(this, &TextBlock::OnTextChanged);
}


//================
// Common Private
//================

VOID TextBlock::OnColorChanged()
{
Invalidate();
}

VOID TextBlock::OnFontChanged()
{
Invalidate(true);
}

VOID TextBlock::OnTextChanged(Handle<String> text)
{
Invalidate(true);
}

}}