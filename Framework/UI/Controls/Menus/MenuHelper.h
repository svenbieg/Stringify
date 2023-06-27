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


//========
// Common
//========

VOID DisableMenuBitmap(Handle<Graphics::Bitmap> Bitmap);
CHAR GetMenuAccelerator(LPCSTR Text);
Handle<String> GetMenuLabel(LPCSTR Text);
Handle<String> GetMenuShortcut(LPCSTR Text);

}}}