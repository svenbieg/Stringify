//========
// Rect.h
//========

#pragma once


//=======
// Using
//=======

#include "Point.h"
#include "Size.h"


//===========
// Namespace
//===========

namespace Graphics {


//======
// Rect
//======

class RECT
{
public:
	// Con-/Destructors
	RECT(): Left(0), Top(0), Right(0), Bottom(0) {}
	RECT(SIZE const& Size): Left(0), Top(0), Right(Size.Width), Bottom(Size.Height) {}
	RECT(INT Left, INT Top, INT Right, INT Bottom): Left(Left), Top(Top), Right(Right), Bottom(Bottom) {}

	// Access
	inline operator bool()const { return (Right!=Left)&&(Bottom!=Top); }
	UINT GetHeight()const;
	SIZE GetSize()const;
	UINT GetWidth()const;

	// Comparison
	inline bool operator==(RECT const& Rect) { return CompareMemory(this, &Rect, sizeof(RECT))==0; }
	inline bool operator!=(RECT const& Rect) { return CompareMemory(this, &Rect, sizeof(RECT))!=0; }
	bool Inside(RECT const& Rect)const;
	bool Intersection(RECT const& Rect)const;

	// Modification
	inline RECT& operator*=(FLOAT Scale) { Left*=Scale; Top*=Scale; Right*=Scale; Bottom*=Scale; return *this; }
	inline RECT& operator/=(FLOAT Scale) { Left/=Scale; Top/=Scale; Right/=Scale; Bottom/=Scale; return *this; }
	inline RECT& operator+=(POINT const& Offset) { Left+=Offset.Left; Top+=Offset.Top; Right+=Offset.Left; Bottom+=Offset.Top; return *this; }
	inline RECT& operator-=(POINT const& Offset) { Left-=Offset.Left; Top-=Offset.Top; Right-=Offset.Left; Bottom-=Offset.Top; return *this; }
	inline VOID Clear() { Left=0; Top=0; Right=0; Bottom=0; }
	inline VOID Move(POINT const& Move) { Left+=Move.Left; Top+=Move.Top; Right+=Move.Left; Bottom+=Move.Top; }
	inline VOID Move(INT X, INT Y) { Left+=X; Top+=Y; Right+=X, Bottom+=Y; }
	VOID Normalize();
	inline VOID Set(INT Left, INT Top, INT Right, INT Bottom) { this->Left=Left; this->Top=Top; this->Right=Right; this->Bottom=Bottom; }
	VOID SetBounds(RECT const& Bounds);
	inline VOID SetMargin(RECT const& Margin) { Left-=Margin.Left; Top-=Margin.Top; Right+=Margin.Right; Bottom+=Margin.Bottom; }
	VOID SetMinSize(SIZE const& MinSize);
	inline VOID SetPadding(RECT const& Padding) { Left+=Padding.Left; Top+=Padding.Top; Right-=Padding.Right; Bottom-=Padding.Bottom; }
	inline VOID SetPadding(INT Left, INT Top, INT Right, INT Bottom) { this->Left+=Left; this->Top+=Top; this->Right-=Right; this->Bottom-=Bottom; }
	VOID SetParent(RECT const& Rect);
	VOID SetPosition(INT Left, INT Top);
	inline VOID SetSize(SIZE const& Size) { Right=Left+Size.Width; Bottom=Top+Size.Height; }

	// Arithmetik
	inline RECT operator+(POINT const& Offset)const { return RECT(Left+Offset.Left, Top+Offset.Top, Right+Offset.Left, Bottom+Offset.Top); }
	inline RECT operator*(FLOAT Scale)const { return RECT(Left*Scale, Top*Scale, Right*Scale, Bottom*Scale); }

	// Common
	INT Left;
	INT Top;
	INT Right;
	INT Bottom;
};

}