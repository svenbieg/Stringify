//=================
// Application.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Application.h"


//===========
// Namespace
//===========

namespace Core {


//========
// Common
//========

Handle<Application> Application::Current;

VOID Application::HandleDispatched()
{
Handle<DispatchedHandler> handler;
cDispatchedHandlers.remove_at(0, &handler);
while(handler)
	{
	handler->Run();
	if(!cDispatchedHandlers.remove_at(0, &handler))
		break;
	}
}

VOID Application::Quit()
{
Running=false;
Handle<DispatchedHandler> handler;
cDispatchedHandlers.remove_at(0, &handler);
while(handler)
	{
	handler->Abort();
	if(!cDispatchedHandlers.remove_at(0, &handler))
		break;
	}
}


//============================
// Con-/Destructors Protected
//============================

Application::Application(LPCSTR name, LPCSTR version):
Name(name),
Running(true),
Version(version)
{
Current=this;
}

}