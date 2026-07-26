//=================
// Application.cpp
//=================

#include "Application.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "Culture/LanguageHelper.h"
#include "Resources/Strings/Exception.h"
#include "Storage/Filesystem/FileHelper.h"
#include "UI/AppWindow.h"
#include "CommandLine.h"
#include "ExceptionHelper.h"
#include "PathHelper.h"

using namespace Concurrency;
using namespace Culture;
using namespace Graphics;
using namespace Resources::Strings;
using namespace Storage::Filesystem;
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
Language::Current=LanguageHelper::GetCurrentLanguage();
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

Handle<String> Application::GetResourcePath()
{
if(m_ResourcePath)
	return m_ResourcePath;
auto cmd_line=CommandLine::Create();
auto exe=cmd_line->Arguments->GetAt(0);
auto exe_path=PathHelper::GetDirectory(exe->Begin());
auto app_x=PathHelper::GetParentDirectory(exe_path->Begin());
auto root=PathHelper::GetParentDirectory(app_x->Begin());
auto lookup=String::Create("%s\\AppX", root);
if(FileHelper::DirectoryExists(lookup->Begin()))
	{
	m_ResourcePath=app_x;
	return m_ResourcePath;
	}
TCHAR current[MAX_PATH];
GetCurrentDirectory(MAX_PATH, current);
app_x=String::Create("%s\\AppX", current);
if(FileHelper::DirectoryExists(app_x->Begin()))
	{
	m_ResourcePath=app_x;
	return m_ResourcePath;
	}
throw NotFoundException();
}

VOID Application::Quit()
{
PostQuitMessage(0);
}

INT Application::Run()
{
auto app_wnd=UI::AppWindow::GetCurrent();
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
auto app=Application::GetCurrent();
auto app_wnd=AppWindow::GetCurrent();
HWND hwnd=app_wnd? app_wnd->GetHandle(): HWND_DESKTOP;
auto name=app->GetName();
MessageBoxT(hwnd, msg, name->Begin(), MB_ICONERROR|MB_OK);
app->UnhandledException(app);
ExitProcess(0);
return 0;
}

Application* Application::s_Current=nullptr;

}