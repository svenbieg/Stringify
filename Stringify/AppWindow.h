//=============
// AppWindow.h
//=============

#pragma once


//=======
// Using
//=======

#include "Desktop/AppWindow.h"
#include "Storage/Streams/InputStream.h"
#include "UI/Controls/TextBox.h"


//===========
// Namespace
//===========

namespace Stringify {


//============
// App-Window
//============

class AppWindow: public Desktop::AppWindow
{
public:
	// Using
	using InputStream=Storage::Streams::InputStream;
	using TextBox=UI::Controls::TextBox;

	// Con-/Destructors
	AppWindow();

	// Common
	Handle<TextBox> ResultBox;

private:
	// Common
	VOID OnPathEditPathChanged(Handle<String> Path);
};

}