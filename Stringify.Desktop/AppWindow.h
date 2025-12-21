//=============
// AppWindow.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/TextBox.h"
#include "UI/AppWindow.h"


//===========
// Namespace
//===========

namespace Stringify {


//============
// App-Window
//============

class AppWindow: public UI::AppWindow
{
public:
	// Using
	using TextBox=UI::Controls::TextBox;

	// Con-/Destructors
	static inline Handle<AppWindow> Create() { return new AppWindow(); }

	// Common
	Handle<TextBox> ResultBox;

private:
	// Con-/Destructors
	AppWindow();

	// Common
	VOID OnPathEditPathChanged(Handle<String> Path);
};

}