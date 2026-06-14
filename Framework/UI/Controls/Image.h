//=========
// Image.h
//=========

#pragma once


//=======
// Using
//=======

#include "Graphics/Bitmap.h"
#include "UI/Controls/Control.h"
#include "DynamicHandle.h"


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

	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<Image> Create(Window* Parent) { return Object::Create<Image>(Parent); }

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	SIZE MaxSize;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	DynamicHandle<Image, Bitmap> Source;

private:
	// Con-/Destructors
	Image(Window* Parent);

	// Common
	VOID OnSourceChanged(Handle<Bitmap> Source);
};

}}