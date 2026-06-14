//==================
// ResourceHelper.h
//==================

#pragma once


//=======
// Using
//=======

#include "Graphics/Bitmap.h"
#include "Graphics/Icon.h"


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
	using Icon=Graphics::Icon;

	// Common
	static Handle<Bitmap> CreateBitmap(Handle<String> Path);
	static Handle<Icon> CreateIcon(Handle<String> Path);
	static BITMAPINFO* GetIcon(UINT Id, UINT Size);
	static UINT GetIconCount(UINT Id);
	static UINT GetIconSize(UINT Id, UINT* Size, UINT Count);

private:
	// Common
	static Handle<String> Lookup(Handle<String> Path);
};

}