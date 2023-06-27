//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Framework/Application.h"
#include "Storage/NamedPipe.h"


//===========
// Namespace
//===========

namespace Desktop {


//=============
// Application
//=============

class Application: public Framework::Application
{
public:
	// Common
	static Application* Current;
	VOID Exit();
	INT Run();
	Event<Application> UnhandledException;

protected:
	// Con-/Destructors
	Application(LPCSTR Name);

private:
	// Common
	VOID OnDispatched();
	static LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* Info);
};

}