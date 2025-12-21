//============
// Bitmap.cpp
//============

#include "Bitmap.h"


//=======
// Using
//=======

#include "MemoryHelper.h"


//===========
// Namespace
//===========

namespace Graphics {


//==================
// Con-/Destructors
//==================

Bitmap::~Bitmap()
{
if(!m_Resource)
	delete m_Buffer;
}


//========
// Common
//========

VOID Bitmap::Clear(COLOR color)
{
if(m_Resource)
	throw InvalidContextException();
UINT* buf=(UINT*)m_Buffer;
UINT size=m_Size/sizeof(UINT);
switch(m_BitsPerPixel)
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
Changed(this);
}

Handle<Bitmap> Bitmap::Copy()const
{
Handle<Bitmap> bmp=new Bitmap(m_Width, m_Height, m_BitsPerPixel);
auto src=Begin();
auto dst=const_cast<BYTE*>(bmp->Begin());
MemoryHelper::Copy(dst, src, m_Size);
return bmp;
}

VOID Bitmap::FillRect(RECT const& rc, COLOR c)
{
if(m_Resource)
	throw InvalidContextException();
if(c.GetAlpha()==0)
	return;
RECT rc_fill(rc);
rc_fill.Left=TypeHelper::Max(rc_fill.Left, 0);
rc_fill.Top=TypeHelper::Max(rc_fill.Top, 0);
rc_fill.Right=TypeHelper::Min(rc_fill.Right, (INT)m_Width);
rc_fill.Bottom=TypeHelper::Min(rc_fill.Bottom, (INT)m_Height);
for(INT y=(INT)rc_fill.Top; y<rc_fill.Bottom; y++)
	{
	for(INT x=(INT)rc_fill.Left; x<rc_fill.Right; x++)
		SetPixel(x, y, c);
	}
Changed(this);
}

COLOR Bitmap::GetPixel(UINT left, UINT top)const
{
auto buf=Begin();
COLOR c=Colors::Black;
switch(m_BitsPerPixel)
	{
	case 1:
		{
		UINT pos=top*m_Pitch+left;
		BYTE mod=(BYTE)(1<<(top&7));
		c.SetMonochrome(buf[pos]&mod);
		break;
		}
	case 24:
		{
		UINT pos=top*m_Pitch+left*3;
		MemoryHelper::Copy(&c, &buf[pos], 3);
		break;
		}
	case 32:
		{
		auto ptr=(UINT const*)buf;
		UINT pos=top*m_Width+left;
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
if(m_Resource)
	throw InvalidContextException();
switch(m_BitsPerPixel)
	{
	case 1:
		{
		UINT pos=top*m_Pitch+left;
		BYTE mod=(BYTE)(1<<(top&7));
		if(c.GetMonochrome())
			{
			m_Buffer[pos]|=mod;
			}
		else
			{
			m_Buffer[pos]&=(BYTE)(~mod);
			}
		break;
		}
	case 24:
		{
		UINT pos=top*m_Pitch+left*3;
		m_Buffer[pos++]=c.GetBlue();
		m_Buffer[pos++]=c.GetGreen();
		m_Buffer[pos++]=c.GetRed();
		break;
		}
	case 32:
		{
		UINT* ptr=(UINT*)m_Buffer;
		UINT pos=top*m_Width+left;
		ptr[pos]=c;
		break;
		}
	default:
		break;
	}
Changed(this);
}


//==================
// Common Protected
//==================

UINT Bitmap::Release()
{
if(m_ReferenceCount==1)
	Destroyed(this);
return Object::Release();
}


//==========================
// Con-/Destructors Private
//==========================

Bitmap::Bitmap(UINT width, UINT height, WORD bpp):
m_BitsPerPixel(bpp),
m_Buffer(nullptr),
m_Height(height),
m_Pitch(0),
m_Resource(nullptr),
m_Size(0),
m_Width(width)
{
m_Pitch=width*bpp/8;
m_Size=m_Height*m_Pitch;
m_Buffer=new BYTE[m_Size];
}

Bitmap::Bitmap(UINT width, UINT height, WORD bpp, LPCSTR resource):
m_BitsPerPixel(bpp),
m_Buffer((BYTE*)resource),
m_Height(height),
m_Pitch(0),
m_Resource(resource),
m_Size(0),
m_Width(width)
{
m_Pitch=width*bpp/8;
m_Size=m_Height*m_Pitch;
}

}