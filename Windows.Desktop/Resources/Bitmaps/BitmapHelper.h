//================
// BitmapHelper.h
//================

#pragma once


//=======
// Using
//=======

#include "Graphics/Bitmap.h"


//===========
// Namespace
//===========

namespace Resources {
	namespace Bitmaps {


//==============
// BitmapHelper
//==============

class BitmapHelper
{
public:
	// Using
	using Bitmap=Graphics::Bitmap;

	// Common
	static Handle<Bitmap> CreateBitmap(Handle<String> Path);
};

}}