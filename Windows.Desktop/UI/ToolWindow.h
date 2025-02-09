//==============
// ToolWindow.h
//==============

#pragma once


//=======
// Using
//=======

#include "Culture/Sentence.h"
#include "DynamicHandle.h"
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
	static inline Handle<ToolWindow> Create(Overlapped* Parent=nullptr) { return new ToolWindow(Parent); }

	// Common
	DynamicHandle<ToolWindow, Sentence> Title;

protected:
	// Con-/Destructors
	ToolWindow(Overlapped* Parent);

private:
	// Common
	VOID OnTitleChanged(Handle<Sentence> Title);
};

}