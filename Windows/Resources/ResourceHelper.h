//==================
// ResourceHelper.h
//==================

#pragma once


//=======
// Using
//=======

#include "Graphics/Bitmap.h"


//===========
// Namespace
//===========

namespace Resources {


//=================
// Resource-Helper
//=================

class ResourceHelper
{
public:
	// Using
	using Bitmap=Graphics::Bitmap;

	// Common
	static Handle<Bitmap> CreateBitmap(Handle<String> Path);
	static BITMAPINFO* GetIcon(UINT Id, UINT Size);
	static UINT GetIconCount(UINT Id);
	static UINT GetIconSize(UINT Id, UINT* Size, UINT Count);

private:
	// Common
	static Handle<String> Lookup(Handle<String> Path);
};

}