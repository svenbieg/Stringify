//===========
// Image.cpp
//===========

#include "Image.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Graphics::SIZE Image::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
SIZE min=MinSize*scale;
if(!Source)
	return min;
SIZE size=Source->GetDimensions();
if(size.Width<min.Width)
	{
	FLOAT f=(FLOAT)min.Width/size.Width;
	size.Width*=f;
	size.Height*=f;
	}
if(size.Height<min.Height)
	{
	FLOAT f=(FLOAT)min.Height/size.Height;
	size.Width*=f;
	size.Height*=f;
	}
SIZE max=MaxSize*scale;
if(max.Width)
	{
	if(size.Width>max.Width)
		{
		FLOAT f=(FLOAT)max.Width/size.Width;
		size.Width*=f;
		size.Height*=f;
		}
	}
if(max.Height)
	{
	if(size.Height>max.Height)
		{
		FLOAT f=(FLOAT)max.Height/size.Height;
		size.Width*=f;
		size.Height*=f;
		}
	}
return size;
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


//==========================
// Con-/Destructors Private
//==========================

Image::Image(Window* parent):
Control(parent),
Source(this)
{
Source.Changed.Add(this, &Image::OnSourceChanged);
}


//================
// Common Private
//================

VOID Image::OnSourceChanged(Handle<Bitmap> source)
{
Invalidate(true);
}

}}