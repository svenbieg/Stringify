//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Collections/StringList.h"
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
	using StringList=Collections::StringList;

	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<Application> Create() { return Object::Create<Application>(); }

	// Common
	static inline Application* GetCurrent() { return s_Current; }
	VOID Open(Handle<String> Path);

private:
	// Con-/Destructors
	Application();

	// Common
	VOID OpenIcon(Handle<String> Path);
	VOID ScanImage(Handle<String> Path);
	VOID Stringify(Handle<StringList> Destination, InputStream* Source);
	Handle<AppWindow> m_Window;
	static Application* s_Current;
};

}