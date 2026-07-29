//==============
// MenuHelper.h
//==============

#pragma once


//=======
// Using
//=======

#include "Graphics/Bitmap.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//=============
// Menu-Helper
//=============

class MenuHelper
{
public:
	// Using
	using Bitmap=Graphics::Bitmap;

	// Common
	static CHAR GetAccelerator(LPCTSTR Text);
	static Handle<Bitmap> GetBitmapDisabled(Handle<Bitmap> Bitmap);
	static Handle<String> GetShortcut(LPCTSTR Text);
	static Handle<String> GetText(LPCTSTR Text);
};

}}}