//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Core/Application.h"
#include "UI/Controls/Input.h"
#include "UI/Frame.h"


//===========
// Namespace
//===========

namespace Framework {


//=============
// Application
//=============

class Application: public Core::Application
{
public:
	// Using
	using Input=UI::Controls::Input;

	// Common
	static Application* Current;
	VOID SetFrame(UI::Frame* Frame) { pFrame=Frame; }

	// Edit
	virtual VOID EditCopy();
	virtual VOID EditCut();
	virtual VOID EditDelete();
	virtual VOID EditPaste();
	virtual VOID EditSelectAll();

protected:
	// Con-/Destructors
	Application(LPCSTR Name, LPCSTR Version="1.0");

	// Common
	Input* GetInputControl();
	UI::Frame* pFrame;
};

}