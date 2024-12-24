//==========
// Rect.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Rect.h"


//===========
// Namespace
//===========

namespace Graphics {


//========
// Access
//========

UINT RECT::GetHeight()const
{
if(Top<=Bottom)
	return (UINT)(Bottom-Top);
return (UINT)(Top-Bottom);
}

SIZE RECT::GetSize()const
{
UINT width=GetWidth();
UINT height=GetHeight();
return SIZE(width, height);
}

UINT RECT::GetWidth()const
{
if(Left<=Right)
	return (UINT)(Right-Left);
return (UINT)(Left-Right);
}


//============
// Comparison
//============

bool RECT::Inside(RECT const& rc)const
{
return (Left>=rc.Left)&&(Top>=rc.Top)&&(Right<=rc.Right)&&(Bottom<=rc.Bottom);
}

bool RECT::Intersection(RECT const& rc)const
{
return (Left<rc.Right)&&(Top<rc.Bottom)&&(Right>=rc.Left)&&(Bottom>=rc.Top);
}


//==============
// Modification
//==============

VOID RECT::Normalize()
{
INT left=Left;
INT top=Top;
Left=TypeHelper::Min(left, Right);
Top=TypeHelper::Min(top, Bottom);
Right=TypeHelper::Max(left, Right);
Bottom=TypeHelper::Max(top, Bottom);
}

VOID RECT::SetBounds(RECT const& rc)
{
Left=TypeHelper::Max(Left, rc.Left);
Top=TypeHelper::Max(Top, rc.Top);
Right=TypeHelper::Min(Right, rc.Right);
Bottom=TypeHelper::Min(Bottom, rc.Bottom);
}

VOID RECT::SetMinSize(SIZE const& min_size)
{
if(Left<=Right)
	{
	UINT width=(UINT)(Right-Left);
	if(width<min_size.Width)
		Right=Left+min_size.Width;
	}
else
	{
	UINT width=(UINT)(Left-Right);
	if(width<min_size.Width)
		Right=Left-min_size.Width;
	}
if(Top<=Bottom)
	{
	UINT height=(UINT)(Bottom-Top);
	if(height<min_size.Height)
		Bottom=Top+min_size.Height;
	}
else
	{
	UINT height=(UINT)(Top-Bottom);
	if(height<min_size.Height)
		Bottom=Top-min_size.Height;
	}
}

VOID RECT::SetParent(RECT const& rc_parent)
{
if(Left<rc_parent.Left)
	{
	INT move=rc_parent.Left-Left;
	Move(move, 0);
	}
if(Top<rc_parent.Top)
	{
	INT move=rc_parent.Top-Top;
	Move(0, move);
	}
if(Right>rc_parent.Right)
	{
	INT move=rc_parent.Right-Right;
	Move(move, 0);
	}
if(Bottom>rc_parent.Bottom)
	{
	INT move=rc_parent.Bottom-Bottom;
	Move(0, move);
	}
}

VOID RECT::SetPosition(INT Left, INT Top)
{
UINT width=GetWidth();
UINT height=GetHeight();
this->Left=Left;
this->Top=Top;
this->Right=Left+width;
this->Bottom=Top+height;
}

}