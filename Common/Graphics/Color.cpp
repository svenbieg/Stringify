//===========
// Color.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Color.h"


//===========
// Namespace
//===========

namespace Graphics {


//===============
// Static Access
//===============

Handle<String> COLOR::ToString(COLOR c)
{
BYTE r=c.GetRed();
BYTE g=c.GetGreen();
BYTE b=c.GetBlue();
BYTE a=c.GetAlpha();
if(a<0xFF)
	return new String("#%02x%02x%02x%02x", a, r, g, b);
return new String("#%02x%02x%02x", r, g, b);
}


//============
// Arithmetik
//============

COLOR& COLOR::operator+=(COLOR c)
{
UINT r1=GetRed();
UINT g1=GetGreen();
UINT b1=GetBlue();
UINT a1=GetAlpha();
UINT r2=c.GetRed();
UINT g2=c.GetGreen();
UINT b2=c.GetBlue();
UINT a2=c.GetAlpha();
UINT r=((255-a2)*r1+a2*r2)/255;
UINT g=((255-a2)*g1+a2*g2)/255;
UINT b=((255-a2)*b1+a2*b2)/255;
uColor=(a1<<24)|(b<<16)|(g<<8)|r;
return *this;
}


//==============
// Modification
//==============

VOID COLOR::SetAlpha(BYTE a)
{
uColor&=0xFFFFFF;
uColor|=(UINT)a<<24;
}

VOID COLOR::SetBlue(BYTE b)
{
uColor&=0xFF00FFFF;
uColor|=(UINT)b<<16;
}

VOID COLOR::SetBrightness(FLOAT f)
{
if(f==1.f)
	return;
BYTE a=GetAlpha();
BYTE r=(BYTE)(GetRed()*f);
BYTE g=(BYTE)(GetGreen()*f);
BYTE b=(BYTE)(GetBlue()*f);
Set(r, g, b, a);
}

VOID COLOR::SetGreen(BYTE g)
{
uColor&=0xFFFF00FF;
uColor|=(UINT)g<<8;
}

VOID COLOR::SetMonochrome(BOOL b)
{
uColor=(UINT)(b? 0xFFFFFFFF: 0xFF000000);
}

VOID COLOR::SetRed(BYTE r)
{
uColor&=0xFFFFFF00;
uColor|=r;
}

}
