//=========
// Theme.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/Theme.h"
#include "Brush.h"
#include "Cursor.h"
#include "Font.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//=======
// Theme
//=======

class Theme: public Graphics::Theme
{
public:
	// Using
	using D2DBrush=Graphics::Direct2D::Brush;
	using D2DCursor=Graphics::Direct2D::Cursor;
	using D2DFont=Graphics::Direct2D::Font;

	// Con-/Destructors
	~Theme();
	static Handle<Theme> Get();

private:
	// Con-/Destructors
	Theme();

	// Common
	inline COLOR SystemColor(INT Index) { return GetSysColor(Index)|0xFF000000; }
	static Theme* s_Current;
};

}}