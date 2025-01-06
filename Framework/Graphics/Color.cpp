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
	return String::Create("#%02x%02x%02x%02x", a, r, g, b);
return String::Create("#%02x%02x%02x", r, g, b);
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
m_Color=(a1<<24)|(b<<16)|(g<<8)|r;
return *this;
}


//==============
// Modification
//==============

VOID COLOR::SetAlpha(BYTE a)
{
m_Color&=0xFFFFFF;
m_Color|=(UINT)a<<24;
}

VOID COLOR::SetBlue(BYTE b)
{
m_Color&=0xFF00FFFF;
m_Color|=(UINT)b<<16;
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
m_Color&=0xFFFF00FF;
m_Color|=(UINT)g<<8;
}

VOID COLOR::SetMonochrome(BOOL b)
{
m_Color=(UINT)(b? 0xFFFFFFFF: 0xFF000000);
}

VOID COLOR::SetRed(BYTE r)
{
m_Color&=0xFFFFFF00;
m_Color|=r;
}

}
