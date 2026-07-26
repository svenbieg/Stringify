//============
// Dialog.cpp
//============

#include "Dialog.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "UI/AppWindow.h"

using namespace Concurrency;
using namespace UI;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Common
//========

INT Dialog::Show()
{
auto app_wnd=AppWindow::GetCurrent();
if(app_wnd)
	{
	auto hwnd=app_wnd->GetHandle();
	EnableWindow(hwnd, false);
	}
Center();
m_Status=Status::Pending;
Overlapped::Show(SW_SHOW);
while(m_Status==Status::Pending)
	{
	MSG msg;
	GetMessage(&msg, NULL, 0, 0);
	if(msg.message==WM_DISPATCH)
		{
		DispatchedQueue::Run();
		continue;
		}
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}
if(app_wnd)
	{
	auto hwnd=app_wnd->GetHandle();
	EnableWindow(hwnd, true);
	}
return 0;
}


//============================
// Con-/Destructors Protected
//============================

Dialog::Dialog():
m_Status(Status::Success)
{}


//==================
// Common Protected
//==================

LRESULT Dialog::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
switch(msg)
	{
	case WM_DESTROY:
		{
		m_Status=Status::Success;
		break;
		}
	case WM_KEYDOWN:
		{
		VirtualKey key=(VirtualKey)LOWORD(wparam);
		if(key==VirtualKey::Escape)
			{
			Close();
			return 0;
			}
		break;
		}
	}
return Overlapped::HandleMessage(msg, wparam, lparam);
}


//================
// Common Private
//================

VOID Dialog::Center()
{
Minimize(Minimization::Size);
RECT rc_app;
auto app_wnd=AppWindow::GetCurrent();
if(app_wnd)
	{
	rc_app=app_wnd->GetRect();
	}
else
	{
	HWND hwnd=GetDesktopWindow();
	::RECT rc_desktop;
	::GetWindowRect(hwnd, &rc_desktop);
	rc_app.Right=rc_desktop.right;
	rc_app.Bottom=rc_desktop.bottom;
	}
UINT app_width=rc_app.GetWidth();
UINT app_height=rc_app.GetHeight();
auto rc=GetRect();
UINT width=rc.GetWidth();
UINT height=rc.GetHeight();
rc.Left=rc_app.Left+(app_width-width)/2;
rc.Top=rc_app.Top+(app_height-height)/4;
rc.Right=rc.Left+width;
rc.Bottom=rc.Top+height;
Move(rc);
}

}}