//============
// Bitmap.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Bitmap.h"


//===========
// Namespace
//===========

namespace Graphics {


//==================
// Con-/Destructors
//==================

Bitmap::Bitmap(UINT width, UINT height, WORD bpp):
pBuffer(nullptr),
pResource(nullptr),
uBitsPerPixel(bpp),
uHeight(height),
uPitch(0),
uSize(0),
uWidth(width)
{
uPitch=width*bpp/8;
uSize=uHeight*uPitch;
pBuffer=new BYTE[uSize];
}

Bitmap::Bitmap(UINT width, UINT height, UINT size, LPCSTR resource):
pBuffer(nullptr),
pResource(resource),
uBitsPerPixel(0),
uHeight(height),
uPitch(size/height),
uSize(size),
uWidth(width)
{
UINT pixel_size=uPitch/width;
uBitsPerPixel=pixel_size*8;
}

Bitmap::~Bitmap()
{
Destroyed(this);
if(pBuffer)
	delete pBuffer;
}


//========
// Common
//========

BYTE const* Bitmap::Begin()const
{
if(pResource)
	return (BYTE const*)pResource;
return pBuffer;
}

VOID Bitmap::Clear(COLOR color)
{
if(!pBuffer)
	return;
UINT* buf=(UINT*)pBuffer;
UINT size=uSize/sizeof(UINT);
switch(uBitsPerPixel)
	{
	case 1:
		{
		UINT c=0;
		if(color.GetMonochrome())
			c=0xFFFFFFFF;
		for(UINT u=0; u<size; u++)
			buf[u]=c;
		break;
		}
	case 24:
		{
		UINT r=color.GetRed();
		UINT g=color.GetGreen();
		UINT b=color.GetBlue();
		UINT c1=(b<<24)|(r<<16)|(g<<8)|b;
		UINT c2=(g<<24)|(b<<16)|(r<<8)|g;
		UINT c3=(r<<24)|(g<<16)|(b<<8)|r;
		for(UINT u=0; u+2<size; u+=3)
			{
			buf[u]=c1;
			buf[u+1]=c2;
			buf[u+2]=c3;
			}
		break;
		}
	case 32:
		{
		UINT c=color.Reverse();
		for(UINT u=0; u<size; u++)
			buf[u]=c;
		break;
		}
	default:
		break;
	}
}

Handle<Bitmap> Bitmap::Copy()const
{
Handle<Bitmap> bmp=new Bitmap(uWidth, uHeight, uBitsPerPixel);
auto src=Begin();
auto dst=const_cast<BYTE*>(bmp->Begin());
CopyMemory(dst, src, uSize);
return bmp;
}

VOID Bitmap::FillRect(RECT const& rc, COLOR c)
{
if(c.GetAlpha()==0)
	return;
RECT rc_fill(rc);
rc_fill.Left=MAX(rc_fill.Left, 0);
rc_fill.Top=MAX(rc_fill.Top, 0);
rc_fill.Right=MIN(rc_fill.Right, (INT)uWidth);
rc_fill.Bottom=MIN(rc_fill.Bottom, (INT)uHeight);
for(INT y=(INT)rc_fill.Top; y<rc_fill.Bottom; y++)
	{
	for(INT x=(INT)rc_fill.Left; x<rc_fill.Right; x++)
		SetPixel(x, y, c);
	}
}

COLOR Bitmap::GetPixel(UINT left, UINT top)const
{
auto buf=Begin();
COLOR c=Colors::Black;
switch(uBitsPerPixel)
	{
	case 1:
		{
		UINT pos=top*uPitch+left;
		BYTE mod=(BYTE)(1<<(top&7));
		c.SetMonochrome(buf[pos]&mod);
		break;
		}
	case 24:
		{
		UINT pos=top*uPitch+left*3;
		CopyMemory(&c, &buf[pos], 3);
		break;
		}
	case 32:
		{
		auto ptr=(UINT const*)buf;
		UINT pos=top*uWidth+left;
		c=ptr[pos];
		break;
		}
	default:
		break;
	}
return c;
}

VOID Bitmap::SetPixel(UINT left, UINT top, COLOR c)
{
if(!pBuffer)
	return;
switch(uBitsPerPixel)
	{
	case 1:
		{
		UINT pos=top*uPitch+left;
		BYTE mod=(BYTE)(1<<(top&7));
		if(c.GetMonochrome())
			{
			pBuffer[pos]|=mod;
			}
		else
			{
			pBuffer[pos]&=(BYTE)(~mod);
			}
		break;
		}
	case 24:
		{
		UINT pos=top*uPitch+left*3;
		pBuffer[pos++]=c.GetBlue();
		pBuffer[pos++]=c.GetGreen();
		pBuffer[pos++]=c.GetRed();
		break;
		}
	case 32:
		{
		UINT* ptr=(UINT*)pBuffer;
		UINT pos=top*uWidth+left;
		ptr[pos]=c;
		break;
		}
	default:
		break;
	}
}

}