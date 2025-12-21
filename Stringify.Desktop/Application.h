//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/Stream.h"
#include "Desktop/Application.h"
#include "AppWindow.h"


//===========
// Namespace
//===========

namespace Stringify {


//=============
// Application
//=============

class Application: public Desktop::Application
{
public:
	// Using
	using IInputStream=Storage::Streams::IInputStream;

	// Con-/Destructors
	static inline Handle<Application> Create() { return new Application(); }

	// Common
	static Application* Current;
	VOID Open(Handle<String> Path);

private:
	// Con-/Destructors
	Application();

	// Common
	VOID OpenBinary(Handle<String> Path);
	VOID OpenIcon(Handle<String> Path);
	VOID StretchOctal(LPSTR Destination, LPCSTR Source);
	VOID Stringify(Handle<String> Name, IInputStream* Source);
	Handle<AppWindow> m_Window;
};

}