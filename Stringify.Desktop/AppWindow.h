//=============
// AppWindow.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/PathEdit.h"
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
	using PathEdit=UI::Controls::PathEdit;
	using TextBox=UI::Controls::TextBox;

	// Con-/Destructors
	static inline Handle<AppWindow> Create() { return new AppWindow(); }

	// Common
	Handle<PathEdit> Path;
	Handle<TextBox> Result;

private:
	// Con-/Destructors
	AppWindow();

	// Common
	VOID OnPathEditPathChanged(Handle<String> Path);
};

}