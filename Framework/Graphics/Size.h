//========
// Size.h
//========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {


//======================
// Forward-Declarations
//======================

class RECT;


//======
// Size
//======

class SIZE
{
public:
	// Con-/Destructors
	SIZE(): Width(0), Height(0) {}
	SIZE(UINT Width, UINT Height): Width(Width), Height(Height) {}

	// Access
	inline operator bool()const { return Width>0||Height>0; }

	// Comparison
	inline bool operator==(SIZE const& Size) { return MemoryHelper::Compare(this, &Size, sizeof(SIZE))==0; }
	inline bool operator!=(SIZE const& Size) { return MemoryHelper::Compare(this, &Size, sizeof(SIZE))!=0; }

	// Modification
	inline SIZE& operator*=(FLOAT Scale) { Width*=Scale; Height*=Scale; return *this; }
	VOID Add(INT X, INT Y) { Width+=X; Height+=Y; }
	VOID AddPadding(RECT const& Padding);
	VOID AddPadding(INT Left, INT Top, INT Right, INT Bottom);
	inline SIZE Max(SIZE const& Size) { return SIZE(TypeHelper::Max(Width, Size.Width), TypeHelper::Max(Height, Size.Height)); }
	inline VOID Set(UINT Width, UINT Height) { this->Width=Width; this->Height=Height; }

	// Arithmetik
	inline SIZE operator*(FLOAT Scale)const { return SIZE(Width*Scale, Height*Scale); }

	// Common
	UINT Width;
	UINT Height;
};

}