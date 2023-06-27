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
	// Con-/Destructors
	Image(Window* Parent=nullptr);

	// Using
	using Bitmap=Graphics::Bitmap;

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	DynamicHandle<Image, Bitmap> Source;

private:
	// Common
	VOID OnSourceChanged(Handle<Bitmap> Source);
};

}}