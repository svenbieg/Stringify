//============
// EditMenu.h
//============

#pragma once


//=======
// Using
//=======

#include "PopupMenu.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//===========
// Edit-Menu
//===========

class EditMenu: public PopupMenu
{
public:
	// Con-/Destructors
	static inline Handle<EditMenu> Create(Window* Parent) { return new EditMenu(Parent); }

	// Common
	Handle<PopupMenuItem> Copy;
	Handle<PopupMenuItem> Cut;
	Handle<PopupMenuItem> Delete;
	Handle<PopupMenuItem> Paste;
	Handle<PopupMenuItem> SelectAll;

private:
	// Con-/Destructors
	EditMenu(Window* Parent);
};

}}}