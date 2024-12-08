//=================
// Application.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Culture/LanguageHelper.h"
#include "Resources/Strings/Exception.h"
#include "UI/AppWindow.h"
#include "Application.h"
#include "ExceptionHelper.h"

using namespace Concurrency;
using namespace Culture;
using namespace Resources::Strings;
using namespace UI;

extern INT Main();


//=============
// Entry-Point
//=============

#ifndef _WINDOWS_CONSOLE

INT g_ShowCommand=0;

INT WINAPI WinMain(HINSTANCE inst, HINSTANCE prev_inst, LPSTR cmd_line, INT show_cmd)
{
SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
CurrentLanguage=GetCurrentLanguage();
g_ShowCommand=show_cmd;
return Main();
}

#endif


//===========
// Namespace
//===========

namespace Desktop {


//========
// Common
//========

Application* Application::Current=nullptr;

VOID Application::DispatchHandler(DispatchedHandler* handler)
{
ScopedLock lock(m_Mutex);
DispatchedHandler::Append(m_DispatchedHandler, handler);
PostThreadMessage(m_ThreadId, WM_DISPATCH, 0, 0);
}

VOID Application::Quit()
{
ScopedLock lock(m_Mutex);
Running=false;
m_DispatchedHandler=nullptr;
PostQuitMessage(0);
}

INT Application::Run()
{
ScopedLock lock(m_Mutex);
auto app_wnd=UI::AppWindow::Current;
if(app_wnd)
	app_wnd->Show(g_ShowCommand);
INT status=0;
MSG msg;
lock.Unlock();
while(GetMessage(&msg, NULL, 0, 0))
	{
	if(msg.message==WM_QUIT)
		{
		status=(INT)msg.wParam;
		break;
		}
	lock.Lock();
	while(m_DispatchedHandler)
		{
		auto handler=DispatchedHandler::Remove(m_DispatchedHandler);
		lock.Unlock();
		handler->Run();
		lock.Lock();
		}
	lock.Unlock();
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}
return status;
}


//============================
// Con-/Destructors Protected
//============================

Application::Application(LPCSTR name, LPCSTR version):
UI::Application(name, version)
{
Current=this;
SetUnhandledExceptionFilter(UnhandledExceptionHandler);
m_ThreadId=GetCurrentThreadId();
}


//================
// Common Private
//================

LONG WINAPI Application::UnhandledExceptionHandler(EXCEPTION_POINTERS* info)
{
LPCSTR caption=Translate(STR_EXCEPTION);
CHAR context[128];
UINT context_len=PrintExceptionContext(info->ContextRecord, 3, context, 128);
CHAR msg[256];
if(context_len>0)
	{
	StringPrint(msg, 256, "%s\n\n%s", caption, context);
	}
else
	{
	StringPrint(msg, 256, "%s", caption);
	}
auto app=Application::Current;
auto app_wnd=AppWindow::Current;
HWND hwnd=app_wnd? app_wnd->GetHandle(): NULL;
MessageBoxA(hwnd, msg, app->Name, MB_ICONERROR|MB_OK);
app->UnhandledException(app);
ExitProcess(0);
return 0;
}

}
