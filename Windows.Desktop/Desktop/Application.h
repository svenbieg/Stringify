//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "UI/Application.h"


//===========
// Namespace
//===========

namespace Desktop {


//=============
// Application
//=============

class Application: public UI::Application
{
public:
	// Common
	static Application* Current;
	VOID DispatchHandler(DispatchedHandler* Handler)override;
	VOID Quit()override;
	INT Run()override;
	Event<Application> UnhandledException;

protected:
	// Con-/Destructors
	Application(LPCSTR Name, LPCSTR Version="1.0");

private:
	// Common
	static LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* Info);
	DWORD m_ThreadId;
};

}
