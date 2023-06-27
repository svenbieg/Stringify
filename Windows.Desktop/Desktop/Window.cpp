//============
// Window.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "Window.h"

using namespace Core;


//===========
// Namespace
//===========

namespace Desktop {


//==================
// Con-/Destructors
//==================

Window::Window(Window* parent):
MinSize(0, 0),
Parent(parent),
Visible(this, false),
// Protected
hWindow(NULL),
// Private
pWndProcCom(NULL)
{
Children=new ChildList();
Message.Add(this, &Window::OnMessage);
Visible.Changed.Add(this, &Window::OnVisibleChanged);
if(Parent)
	Parent->Children->Append(this);
}

Window::~Window()
{
if(hWindow)
	DestroyWindow(hWindow);
}


//========
// Common
//========

VOID Window::BringToFront()
{
if(Parent)
	{
	auto children=Parent->Children;
	children->Remove(this, false);
	children->Append(this, false);
	}
SetWindowPos(hWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
Invalidate();
}

VOID Window::Destroy()
{
DestroyWindow(hWindow);
}

Graphics::RECT Window::GetClientRect()const
{
::RECT rc;
::GetClientRect(hWindow, &rc);
return RECT(rc.left, rc.top, rc.right, rc.bottom);
}

Graphics::SIZE Window::GetMinSize()
{
return MinSize;
}

Graphics::RECT Window::GetRect()const
{
::RECT rc;
GetWindowRect(hWindow, &rc);
return RECT(rc.left, rc.top, rc.right, rc.bottom);
}

FLOAT Window::GetScaleFactor()const
{
UINT dpi=GetDpiForWindow(hWindow);
FLOAT scale=(FLOAT)dpi/96;
return scale;
}

BOOL Window::GetStyle(UINT style)
{
UINT styles=GetWindowLong(hWindow, GWL_STYLE);
return (styles&style)!=0;
}

Handle<Window> Window::GetVisibleChild(UINT id)
{
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	if(id==0)
		return child;
	id--;
	}
return nullptr;
}

VOID Window::Invalidate()
{
InvalidateRect(hWindow, nullptr, true);
}

VOID Window::InvalidateSize()
{
}

VOID Window::Move(RECT const& rc)
{
INT left=rc.Left;
INT top=rc.Top;
UINT width=rc.GetWidth();
UINT height=rc.GetHeight();
MoveWindow(hWindow, left, top, width, height, true);
}

VOID Window::Repaint()
{
InvalidateRect(hWindow, nullptr, true);
UpdateWindow(hWindow);
}

VOID Window::SetStyle(UINT style, BOOL set)
{
UINT styles=GetWindowLong(hWindow, GWL_STYLE);
SetFlag(styles, style, set);
SetWindowLong(hWindow, GWL_STYLE, styles);
}


//==================
// Common Protected
//==================

VOID Window::Create(LPCTSTR class_name, UINT class_style, UINT style, UINT style_ex)
{
BOOL visible=GetFlag(style, WS_VISIBLE);
Visible.Set(visible, false);
HINSTANCE inst=GetModuleHandle(nullptr);
WNDCLASSEX wc;
ZeroMemory(&wc, sizeof(WNDCLASSEX));
wc.cbSize=sizeof(WNDCLASSEX);
BOOL com_class=GetClassInfoEx(inst, class_name, &wc);
if(com_class)
	{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(&WndProc, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	VOID* p=mbi.AllocationBase;
	VirtualQuery(wc.lpfnWndProc, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	if(p==mbi.AllocationBase)
		com_class=false;
	}
if(!com_class)
	{
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize=sizeof(WNDCLASSEX);
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hInstance=inst;
	wc.lpfnWndProc=&WndProc;
	wc.lpszClassName=class_name;
	wc.style=class_style;
	SetLastError(0);
	RegisterClassEx(&wc);
	}
HWND parent=HWND_MESSAGE;
if(Parent)
	{
	parent=Parent->GetHandle();
	style|=WS_CHILD;
	}
hWindow=CreateWindowEx(style_ex, class_name, nullptr, style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, NULL, inst, this);
if(hWindow==INVALID_HANDLE_VALUE)
	hWindow=NULL;
if(hWindow==NULL)
	return;
if(com_class)
	{
	SetPropA(hWindow, "WindowHandle", this);
	pWndProcCom=(WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)WndProcCom);
	}
}


//================
// Common Private
//================

VOID Window::OnMessage(WNDMSG& msg)
{
switch(msg.Message)
	{
	case WM_CREATE:
		{
		Created(this);
		msg.Handled=true;
		break;
		}
	case WM_DESTROY:
		{
		Destroyed(this);
		hWindow=NULL;
		msg.Handled=true;
		break;
		}
	case WM_SIZE:
		{
		SizeChanged(this);
		break;
		}
	}
}

VOID Window::OnVisibleChanged(BOOL visible)
{
ShowWindow(hWindow, visible? SW_SHOW: SW_HIDE);
if(visible)
	Invalidate();
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
auto app=Application::Current;
app->HandleDispatched();
auto window=(Window*)GetPropA(hwnd, "WindowHandle");
if(umsg==WM_CREATE)
	{
	CREATESTRUCT* cs=(CREATESTRUCT*)lparam;
	window=(Window*)cs->lpCreateParams;
	SetPropA(hwnd, "WindowHandle", window);
	window->hWindow=hwnd;
	}
if(!window)
	return DefWindowProc(hwnd, umsg, wparam, lparam);
WNDMSG msg;
msg.Handled=false;
msg.LParam=lparam;
msg.Message=umsg;
msg.Status=S_OK;
msg.WParam=wparam;
window->Message(window, msg);
if(msg.Handled)
	return msg.Status;
return DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT CALLBACK Window::WndProcCom(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
auto window=(Window*)GetPropA(hwnd, "WindowHandle");
if(!window)
	return DefWindowProc(hwnd, umsg, wparam, lparam);
WNDMSG msg;
msg.Handled=false;
msg.LParam=lparam;
msg.Message=umsg;
msg.Status=S_OK;
msg.WParam=wparam;
window->Message(window, msg);
if(msg.Handled)
	return msg.Status;
return CallWindowProc(window->pWndProcCom, hwnd, umsg, wparam, lparam);
}

}