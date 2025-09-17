//=================
// Application.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "Culture/LanguageHelper.h"
#include "Resources/Strings/Exception.h"
#include "UI/AppWindow.h"
#include "Application.h"
#include "ExceptionHelper.h"

using namespace Concurrency;
using namespace Culture;
using namespace Graphics;
using namespace Resources::Strings;
using namespace UI;

extern INT Main();


//=============
// Entry-Point
//=============

INT g_ShowCommand=0;

#ifndef _WINDOWS_CONSOLE

INT WINAPI WinMain(HINSTANCE inst, HINSTANCE prev_inst, LPSTR cmd_line, INT show_cmd)
{
SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
DispatchedQueue::Initialize();
Language::Current=GetCurrentLanguage();
g_ShowCommand=show_cmd;
return Main();
}

#endif


//===========
// Namespace
//===========

namespace Desktop {


//==================
// Con-/Destructors
//==================

Application::~Application()
{
s_Current=nullptr;
}


//========
// Common
//========

INT Application::Run()
{
auto app_wnd=UI::AppWindow::Get();
if(app_wnd)
	app_wnd->Show(g_ShowCommand);
INT status=0;
MSG msg;
while(GetMessage(&msg, NULL, 0, 0))
	{
	if(msg.message==WM_DISPATCH)
		{
		DispatchedQueue::Run();
		continue;
		}
	if(msg.message==WM_QUIT)
		{
		status=(INT)msg.wParam;
		break;
		}
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}
DispatchedQueue::Run();
return status;
}


//============================
// Con-/Destructors Protected
//============================

Application::Application(LPCSTR name):
UI::Application(name)
{
s_Current=this;
SetUnhandledExceptionFilter(UnhandledExceptionHandler);
}


//================
// Common Private
//================

LONG WINAPI Application::UnhandledExceptionHandler(EXCEPTION_POINTERS* info)
{
auto caption=Sentence::Translate(STR_EXCEPTION);
CHAR context[128];
UINT context_len=PrintExceptionContext(info->ContextRecord, 3, context, 128);
TCHAR msg[256];
if(context_len>0)
	{
	StringHelper::Print(msg, 256, "%s\n\n%s", caption, context);
	}
else
	{
	StringHelper::Print(msg, 256, "%s", caption);
	}
auto app=Application::Get();
auto app_wnd=AppWindow::Get();
HWND hwnd=app_wnd? app_wnd->GetHandle(): NULL;
auto name=app->GetName();
MessageBoxT(hwnd, msg, name->Begin(), MB_ICONERROR|MB_OK);
app->UnhandledException(app);
ExitProcess(0);
return 0;
}

Application* Application::s_Current=nullptr;

}