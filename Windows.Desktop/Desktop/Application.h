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
	// Con-/Destructors
	~Application();

	static inline Application* GetCurrent() { return s_Current; }
	VOID Quit()override;
	Handle<String> GetResourcePath();
	INT Run();
	Event<Application> UnhandledException;

protected:
	// Con-/Destructors
	Application(LPCSTR Name);

private:
	// Common
	static LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* Info);
	Handle<String> m_ResourcePath;
	Handle<Theme> m_Theme;
	static Application* s_Current;
};

}