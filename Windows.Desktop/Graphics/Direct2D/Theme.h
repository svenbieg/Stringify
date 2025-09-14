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
	static inline Handle<Theme> Create(ColorScheme Scheme=ColorScheme::Light) { return new Theme(Scheme); }

private:
	// Con-/Destructors
	Theme(ColorScheme Scheme);
};

}}