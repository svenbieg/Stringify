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
	INT Run();
	Event<Application> UnhandledException;

protected:
	// Con-/Destructors
	Application(LPCSTR Name);

private:
	// Common
	static LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* Info);
	DWORD m_ThreadId;
};

}
