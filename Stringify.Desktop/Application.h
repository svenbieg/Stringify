//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/InputStream.h"
#include "Storage/Intermediate.h"
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
	using InputStream=Storage::Streams::InputStream;
	using Intermediate=Storage::Intermediate;
	using OutputStream=Storage::Streams::OutputStream;
	using StringList=Collections::StringList;
	using Task=Concurrency::Task;

	// Con-/Destructors
	static inline Handle<Application> Create() { return new Application(); }

	// Common
	static Application* Current;
	VOID Open(Handle<String> Path);

private:
	// Con-/Destructors
	Application();

	// Common
	VOID DoParse(Handle<Intermediate> Stream);
	VOID OpenBinary(Handle<String> Path);
	VOID OpenIcon(Handle<String> Path);
	VOID StretchOctal(LPSTR Destination, LPCSTR Source);
	VOID Stringify(Handle<String> Name, InputStream* Source);
	Handle<Task> m_ConvertTask;
	Concurrency::Mutex m_Mutex;
	Handle<Task> m_ParseTask;
	Handle<AppWindow> m_Window;
};

}
