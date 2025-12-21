//=========
// Point.h
//=========

#pragma once


//=======
// Using
//=======

#include "TypeHelper.h"


//===========
// Namespace
//===========

namespace Graphics {


//======================
// Forward-Declarations
//======================

class RECT;


//=======
// Point
//=======

class POINT
{
public:
	// Con-/Destructors
	POINT(): Left(0), Top(0) {}
	POINT(INT Left, INT Top): Left(Left), Top(Top) {}

	// Assignment
	inline POINT& operator=(POINT const& Point) { Set(Point); return *this; }
	inline VOID Set(POINT const& Point) { Left=Point.Left; Top=Point.Top; }
	inline VOID Set(INT Left, INT Top) { this->Left=Left; this->Top=Top; }

	// Comparison
	inline bool operator==(POINT const& Point)const { return (Left==Point.Left)&&(Top==Point.Top); }
	inline bool operator!=(POINT const& Point)const { return (Left!=Point.Left)||(Top!=Point.Top); }

	// Modification
	inline POINT& operator*=(FLOAT Scale) { Left*=Scale; Top*=Scale; return *this; }
	inline POINT& operator/=(FLOAT Scale) { Left/=Scale; Top/=Scale; return *this; }
	inline POINT& operator+=(POINT const& Point) { Left+=Point.Left; Top+=Point.Top; return *this; }
	inline POINT& operator-=(POINT const& Point) { Left-=Point.Left; Top-=Point.Top; return *this; }
	inline VOID Clear() { Left=0; Top=0; }

	// Arithmetik
	inline POINT operator*(FLOAT Scale) { return POINT(Left*Scale, Top*Scale); }
	inline POINT operator/(FLOAT Scale) { return POINT(Left/Scale, Top/Scale); }
	inline POINT operator+(POINT const& Point) { return POINT(Left+Point.Left, Top+Point.Top); }
	inline POINT operator-(POINT const& Point) { return POINT(Left-Point.Left, Top-Point.Top); }

	// Common
	BOOL Inside(RECT const& Rect)const;
	INT Left;
	INT Top;
};

}