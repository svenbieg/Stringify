//===========
// Image.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Image.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Image::Image(Window* parent):
Control(parent),
Source(this)
{
Source.Changed.Add(this, &Image::OnSourceChanged);
}


//========
// Common
//========

Graphics::SIZE Image::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
if(!Source)
	return MinSize*scale;
SIZE size=Source->GetDimensions();
size*=Scale;
return size.Max(MinSize*scale);
}

VOID Image::Render(RenderTarget* target, RECT& rc)
{
Control::Render(target, rc);
if(!Source)
	return;
SIZE size=Source->GetDimensions();
RECT rc_src(size);
target->DrawBitmap(rc, Source, rc_src);
}


//================
// Common Private
//================

VOID Image::OnSourceChanged(Handle<Bitmap> source)
{
Invalidate(true);
}

}}