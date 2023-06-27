//================
// Overlapped.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "Overlapped.h"


//===========
// Namespace
//===========

namespace Desktop {


//==================
// Con-/Destructors
//==================

Overlapped::Overlapped(Window* parent, Handle<D2DIcon> icon):
Frame(parent),
CloseButton(this, true),
Title(this),
hIcon(icon)
{
Created.Add(this, &Overlapped::OnCreated);
Message.Add(this, &Overlapped::OnMessage);
}


//========
// Common
//========

VOID Overlapped::Activate()
{
WINDOWPLACEMENT place;
ZeroMemory(&place, sizeof(WINDOWPLACEMENT));
place.length=sizeof(WINDOWPLACEMENT);
GetWindowPlacement(hWindow, &place);
if(place.showCmd==SW_SHOWMINIMIZED)
	place.showCmd=SW_RESTORE;
ShowWindow(hWindow, place.showCmd);
SetWindowPos(hWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
SetForegroundWindow(hWindow);
}

VOID Overlapped::Center()
{
if(!hWindow)
	return;
RECT rc=GetRect();
UINT width=rc.GetWidth();
UINT height=rc.GetHeight();
HWND hwnd=GetDesktopWindow();
::RECT rc_desktop;
GetWindowRect(hwnd, &rc_desktop);
UINT desktop_width=rc_desktop.right-rc_desktop.left;
UINT desktop_height=(rc_desktop.bottom-rc_desktop.top)*0.8f;
rc.Left=(desktop_width-width)/2;
rc.Top=(desktop_height-height)/2;
rc.Right=rc.Left+width;
rc.Bottom=rc.Top+height;
Move(rc);
}

VOID Overlapped::Create(LPCTSTR class_name, UINT style, UINT style_ex)
{
style|=WS_CLIPCHILDREN;
style_ex|=WS_EX_CONTROLPARENT;
Window::Create(class_name, 0, style, style_ex);
HWND desktop=GetDesktopWindow();
SetParent(hWindow, desktop);
SetStyle(WS_CHILD, false);
if(!CloseButton)
	OnCloseButtonChanged(false);
if(Title)
	OnTitleChanged(Title);
}

Graphics::RECT Overlapped::GetBorderWidth()
{
if(!hWindow)
	return RECT();
::RECT rc_client;
::GetClientRect(hWindow, &rc_client);
::RECT rc_wnd(rc_client);
UINT style=GetWindowLongA(hWindow, GWL_STYLE);
UINT style_ex=GetWindowLongA(hWindow, GWL_EXSTYLE);
AdjustWindowRectEx(&rc_wnd, style, GetMenu(hWindow)!=NULL, style_ex);
RECT rc_border;
rc_border.Left=(rc_client.left-rc_wnd.left);
rc_border.Right=(rc_wnd.right-rc_client.right);
rc_border.Top=(rc_client.top-rc_wnd.top);
rc_border.Bottom=(rc_wnd.bottom-rc_client.bottom);
return rc_border;
}

Graphics::SIZE Overlapped::GetMinSize()
{
RECT rc_border=GetBorderWidth();
SIZE size;
size.Width=rc_border.Left+rc_border.Right;
size.Height=rc_border.Top+rc_border.Bottom;
SIZE content_size=UIFrame->GetMinSize(hRenderTarget);
size.Width+=content_size.Width;
size.Height+=content_size.Height;
FLOAT scale=GetScaleFactor();
return size.Max(MinSize*scale);
}

VOID Overlapped::MinimizeSize()
{
if(!hWindow)
	return;
SIZE size=GetMinSize();
RECT rc=GetRect();
rc.Right=rc.Left+size.Width;
rc.Bottom=rc.Top+size.Height;
Move(rc);
}


//================
// Common Private
//================

VOID Overlapped::OnCloseButtonChanged(BOOL btn)
{
if(btn)
	return;
HMENU hmenu=GetSystemMenu(hWindow, false);
DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
DrawMenuBar(hWindow);
}

VOID Overlapped::OnCreated()
{
CloseButton.Changed.Add(this, &Overlapped::OnCloseButtonChanged);
Title.Changed.Add(this, &Overlapped::OnTitleChanged);
if(hIcon)
	{
	UINT size_small=GetSystemMetrics(SM_CXSMICON);
	UINT size_big=GetSystemMetrics(SM_CXICON);
	HICON ico_small=hIcon->GetHandle(size_small);
	HICON ico_big=hIcon->GetHandle(size_big);
	SendMessage(hWindow, WM_SETICON, ICON_SMALL, (LPARAM)ico_small);
	SendMessage(hWindow, WM_SETICON, ICON_BIG, (LPARAM)ico_big);
	}
}

VOID Overlapped::OnMessage(WNDMSG& msg)
{
switch(msg.Message)
	{
	case WM_ACTIVATE:
		{
		Activated(this);
		msg.Handled=true;
		break;
		}
	case WM_CLOSE:
		{
		if(Closed)
			{
			Closed(this);
			}
		else
			{
			Destroy();
			}
		msg.Handled=true;
		break;
		}
	case WM_SIZE:
		{
		auto content=GetVisibleChild(0);
		if(content)
			{
			RECT rc=GetClientRect();
			content->Move(rc);
			}
		switch(msg.WParam)
			{
			case SIZE_MAXIMIZED:
				{
				Maximized(this);
				break;
				}
			case SIZE_MINIMIZED:
				{
				Minimized(this);
				break;
				}
			}
		msg.Handled=true;
		break;
		}
	case WM_SIZING:
		{
		auto rc_ptr=(::RECT*)msg.LParam;
		::RECT& rc=*rc_ptr;
		SIZE size;
		size.Width=rc.right-rc.left;
		size.Height=rc.bottom-rc.top;
		SIZE min_size=GetMinSize();
		size=size.Max(min_size);
		WPARAM wparam=msg.WParam;
		if(wparam==WMSZ_RIGHT||wparam==WMSZ_TOPRIGHT||wparam==WMSZ_BOTTOMRIGHT)
			{
			rc.right=rc.left+size.Width;
			}
		else
			{
			rc.left=rc.right-size.Width;
			}
		if(wparam==WMSZ_BOTTOM||wparam==WMSZ_BOTTOMLEFT||wparam==WMSZ_BOTTOMRIGHT)
			{
			rc.bottom=rc.top+size.Height;
			}
		else
			{
			rc.top=rc.bottom-size.Height;
			}
		msg.Handled=true;
		break;
		}
	}
}

VOID Overlapped::OnTitleChanged(Handle<Sentence> title)
{
SetWindowTextA(hWindow, title? title->Begin(): "");
}

}