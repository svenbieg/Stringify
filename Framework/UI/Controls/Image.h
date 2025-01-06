//=========
// Image.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/Bitmap.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//=======
// Image
//=======

class Image: public Control
{
public:
	// Using
	using Bitmap=Graphics::Bitmap;

	// Con-/Destructors
	static inline Handle<Image> Create(Window* Parent) { return new Image(Parent); }

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	DynamicHandle<Image, Bitmap> Source;

private:
	// Con-/Destructors
	Image(Window* Parent);

	// Common
	VOID OnSourceChanged(Handle<Bitmap> Source);
};

}}