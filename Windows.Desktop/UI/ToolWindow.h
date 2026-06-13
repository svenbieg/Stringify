//==============
// ToolWindow.h
//==============

#pragma once


//=======
// Using
//=======

#include "Culture/Sentence.h"
#include "UI/Overlapped.h"
#include "DynamicHandle.h"


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

	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<ToolWindow> Create()
		{
		return Object::Create<ToolWindow>();
		}

	// Common
	DynamicHandle<ToolWindow, Sentence> Title;

protected:
	// Con-/Destructors
	ToolWindow();

private:
	// Common
	VOID OnTitleChanged(Handle<Sentence> Title);
};

}