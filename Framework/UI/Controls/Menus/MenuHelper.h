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
	// Common
	static CHAR GetAccelerator(LPCTSTR Text);
	static VOID GetBitmapDisabled(Handle<Graphics::Bitmap> Bitmap);
	static Handle<String> GetLabel(LPCTSTR Text);
	static Handle<String> GetShortcut(LPCTSTR Text);
};

}}}