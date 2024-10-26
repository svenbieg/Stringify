//=============
// AppWindow.h
//=============

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/InputStream.h"
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