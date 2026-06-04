//============
// EditMenu.h
//============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Menus/PopupMenu.h"


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
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<EditMenu> Create() { return Object::Create<EditMenu>(); }

	// Common
	Handle<PopupMenuItem> Copy;
	Handle<PopupMenuItem> Cut;
	Handle<PopupMenuItem> Delete;
	Handle<PopupMenuItem> Paste;
	Handle<PopupMenuItem> SelectAll;

private:
	// Con-/Destructors
	EditMenu();
};

}}}