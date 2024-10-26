//=========
// Color.h
//=========

#pragma once


//===========
// Namespace
//===========

namespace Graphics {


//========
// Colors
//========

enum class Colors: UINT
{
Black=0xFF000000,
BlackGray=0xFF040404,
Blue=0xFFFF0000,
DarkBlue=0xFF073C72,
DarkGray=0xFF808080,
DarkGreen=0xFF40B020,
Focus=0x20F4DEC8,
FocusBorder=0x40F4DEC8,
Gray=0xFFA0A0A0,
Green=0xFF00FF00,
Highlight=0xFFF4DEC8,
HighlightBorder=0xFFFF8040,
LightGray=0xFFD0D0D0,
Orange=0xFF00A5FF,
Red=0xFF0000FF,
Silver=0xFFE0E0E0,
Transparent=0,
Violett=0xFFFF0088,
Yellow=0xFF00FFFF,
White=0xFFFFFFFF,
WhiteGray=0xFFFCFCFC
};


//=======
// Color
//=======

class COLOR
{
public:
	// Con-/Destructors
	COLOR(UINT Color=0): uColor(Color) {}
	COLOR(Colors Color): uColor((UINT)Color) {}
	COLOR(BYTE R, BYTE G, BYTE B, BYTE A=0xFF) { Set(R, G, B, A); }

	// Static Access
	static inline BYTE GetAlpha(COLOR Color) { return (BYTE)(Color>>24); }
	static inline BYTE GetBlue(COLOR Color) { return (BYTE)(Color>>16); }
	static inline BYTE GetGreen(COLOR Color) { return (BYTE)(Color>>8); }
	static inline BOOL GetMonochrome(COLOR Color) { return Color>=0xFF808080; }
	static inline BYTE GetRed(COLOR Color) { return (BYTE)Color; }
	static inline BOOL IsTransparent(COLOR Color) { return GetAlpha(Color)<0xFF; }
	static inline UINT Reverse(COLOR Color) { return (Color<<24)|(Color&0xFF00<<8)|(Color&0xFF0000>>8)|(Color>>24); }
	static Handle<String> ToString(COLOR Color);

	// Access
	inline operator UINT()const { return uColor; }
	inline BYTE GetAlpha()const { return (BYTE)(uColor>>24); }
	inline BYTE GetBlue()const { return (BYTE)(uColor>>16); }
	inline BYTE GetGreen()const { return (BYTE)(uColor>>8); }
	inline BOOL GetMonochrome()const { return uColor>=0xFF808080; }
	inline BYTE GetRed()const { return (BYTE)uColor; }
	inline BOOL IsTransparent()const { return GetAlpha()<0xFF; }
	inline UINT Reverse()const { return Reverse(uColor); }
	inline UINT GetRGB()const { return uColor&0xFFFFFF; }
	inline Handle<String> ToString()const { return ToString(uColor); }

	// Arithmetik
	COLOR& operator+=(COLOR Color);
	inline COLOR operator+(COLOR Color) { COLOR c(uColor); c+=Color; return c; }

	// Modification
	inline VOID Set(BYTE R, BYTE G, BYTE B, BYTE A=0xFF) { uColor=((UINT)A<<24)|((UINT)B<<16)|((UINT)G<<8)|R; }
	VOID SetAlpha(BYTE Alpha);
	VOID SetBlue(BYTE Blue);
	VOID SetBrightness(FLOAT Brightness);
	VOID SetGreen(BYTE Green);
	VOID SetMonochrome(BOOL Monochrome);
	VOID SetRed(BYTE Red);

private:
	// Common
	UINT uColor;
};

}
