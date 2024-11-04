//==============
// ToolWindow.h
//==============

#pragma once


//=======
// Using
//=======

#include "Overlapped.h"


//===========
// Namespace
//===========

namespace UI {


//=============
// Tool-Window
//=============

class ToolWindow: public Overlapped
{
public:
	// Using
	using Sentence=Culture::Sentence;

	// Con-/Destructors
	ToolWindow();
	ToolWindow(HWND Parent);
	ToolWindow(Window* Parent);
	ToolWindow(Frame* Parent);
	ToolWindow(Overlapped* Parent);

	// Common
	DynamicHandle<ToolWindow, Sentence> Title;

private:
	// Common
	VOID OnTitleChanged(Handle<Sentence> Title);
};

}