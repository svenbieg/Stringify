//================
// Overlapped.cpp
//================

#include "Overlapped.h"


//=======
// Using
//=======

#pragma comment(lib, "dwmapi")

#include <dwmapi.h>
#include <windowsx.h>
#include "Concurrency/DispatchedQueue.h"
#include "Desktop/Application.h"
#include "UI/AppWindow.h"

using namespace Concurrency;
using namespace Desktop;
using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {


//==================
// Con-/Destructors
//==================

Overlapped::~Overlapped()
{
if(m_Theme)
	m_Theme->Changed.Remove(this);
}


//========
// Common
//========

VOID Overlapped::Activate(FocusReason reason)
{
if(m_Handle)
	SetActiveWindow(m_Handle);
}

VOID Overlapped::BringToFront()
{
if(m_Handle!=NULL)
	SetWindowPos(m_Handle, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
}

VOID Overlapped::Close()
{
if(m_Handle!=NULL)
	SendMessage(m_Handle, WM_CLOSE, 0, 0);
}

Handle<Brush> Overlapped::GetBackground()
{
return m_Theme->ControlBrush;
}

Graphics::RECT Overlapped::GetBorderWidth()const
{
if(m_Handle==NULL)
	return RECT();
::RECT rc({ 0, 0, 0, 0});
UINT style=GetWindowLongA(m_Handle, GWL_STYLE);
UINT style_ex=GetWindowLongA(m_Handle, GWL_EXSTYLE);
UINT dpi=GetDpiForWindow(m_Handle);
AdjustWindowRectExForDpi(&rc, style, false, style_ex, dpi);
return RECT(-rc.left, -rc.top, rc.right, rc.bottom);
}

Graphics::RECT Overlapped::GetClientRect()const
{
RECT border=GetBorderWidth();
UINT width=m_Rect.GetWidth()-border.Left-border.Right;
UINT height=m_Rect.GetHeight()-border.Bottom-border.Top;
return RECT(0, 0, width, height);
}

Graphics::SIZE Overlapped::GetMinSize(RenderTarget* target)
{
SIZE min_size=Frame::GetMinSize(target);
RECT border=GetBorderWidth();
min_size.Width+=border.Left+border.Right;
min_size.Height+=border.Top+border.Bottom;
return min_size;
}

RenderTarget* Overlapped::GetRenderTarget()const
{
return m_RenderTarget;
}

Graphics::POINT Overlapped::GetScreenOffset()const
{
POINT pt=Frame::GetScreenOffset();
RECT border=GetBorderWidth();
pt.Left+=border.Left;
pt.Top+=border.Top;
return pt;
}

VOID Overlapped::Minimize()
{
if(m_Handle!=NULL)
	ShowWindow(m_Handle, SW_MINIMIZE);
}

VOID Overlapped::Minimize(Minimization)
{
SIZE size=GetMinSize(m_RenderTarget);
RECT rc(m_Rect);
rc.SetSize(size);
Move(rc);
}

VOID Overlapped::Move(RECT const& rc)
{
if(m_Handle==NULL)
	return;
SIZE size=GetMinSize(m_RenderTarget);
size.Max(rc.GetSize());
MoveWindow(m_Handle, rc.Left, rc.Top, size.Width, size.Height, true);
}

VOID Overlapped::Repaint()
{
if(m_Handle!=NULL)
	{
	InvalidateRect(m_Handle, nullptr, false);
	UpdateWindow(m_Handle);
	}
}

VOID Overlapped::SetCursor(Cursor* cursor)
{
if(m_Handle==NULL)
	return;
HCURSOR hcur=NULL;
if(cursor)
	hcur=cursor->GetHandle();
SetClassLongPtr(m_Handle, GCLP_HCURSOR, (LONG_PTR)hcur);
}

VOID Overlapped::SetPointerCapture(Interactive* capture)
{
if(capture)
	{
	m_PointerCapture=capture;
	SetCapture(m_Handle);
	}
else
	{
	m_PointerCapture=nullptr;
	ReleaseCapture();
	}
}

VOID Overlapped::Show(INT show)
{
BOOL visible=!(show==SW_HIDE);
Visible.Set(visible, EventNotification::None);
if(m_Handle!=NULL)
	ShowWindow(m_Handle, show);
}


//============================
// Con-/Destructors Protected
//============================

Overlapped::Overlapped(Overlapped* parent):
Icon(this),
Title(this),
m_Cursor(NULL),
m_Handle(NULL),
m_IconBig(NULL),
m_IconSmall(NULL),
m_Parent(parent)
{
Icon.Changed.Add(this, &Overlapped::OnIconChanged);
Title.Changed.Add(this, &Overlapped::OnTitleChanged);
m_Cursor=LoadCursor(NULL, IDC_ARROW);
m_RenderTarget=RenderTarget::Create();
Invalidated.Add(this, &Overlapped::OnInvalidated);
Visible.Changed.Add(this, &Overlapped::OnVisibleChanged);
Visible.Set(false, EventNotification::None);
LPCTSTR class_name=TEXT("Overlapped");
HINSTANCE inst=GetModuleHandle(nullptr);
WNDCLASSEX wc={ 0 };
wc.cbSize=sizeof(WNDCLASSEX);
wc.hCursor=m_Cursor;
wc.hInstance=inst;
wc.lpfnWndProc=&WndProc;
wc.lpszClassName=class_name;
wc.style=CS_HREDRAW|CS_VREDRAW;
SetLastError(0);
RegisterClassEx(&wc);
HWND hwnd_parent=HWND_DESKTOP;
if(!m_Parent)
	m_Parent=AppWindow::GetCurrent();
if(m_Parent)
	hwnd_parent=m_Parent->GetHandle();
UINT style=WS_OVERLAPPED;
m_Handle=CreateWindowEx(0, class_name, nullptr, style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd_parent, NULL, inst, this);
if(m_Handle==INVALID_HANDLE_VALUE)
	m_Handle=NULL;
if(m_Handle==NULL)
	throw InvalidArgumentException();
if(m_Theme)
	{
	m_Theme->Changed.Add(this, &Overlapped::OnThemeChanged);
	OnThemeChanged();
	}
}


//==================
// Common Protected
//==================

LRESULT Overlapped::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
switch(msg)
	{
	case WM_ACTIVATE:
		{
		if(wparam==WA_INACTIVE)
			break;
		auto reason=FocusReason::None;
		if(wparam==WA_CLICKACTIVE)
			reason=FocusReason::Pointer;
		Frame::Activate(reason);
		break;
		}
	case WM_CHAR:
	case WM_SYSCHAR:
		{
		TCHAR c=(TCHAR)wparam;
		if(c<' ')
			break;
		VirtualKey key=VirtualKey::None;
		auto args=KeyEventArgs::Create(key, c);
		if(DoKey(KeyEventType::KeyDown, args))
			return 0;
		break;
		}
	case WM_CREATE:
		{
		UINT dpi=GetDpiForWindow(m_Handle);
		Scale=(FLOAT)dpi/96;
		::RECT rc;
		::GetWindowRect(m_Handle, &rc);
		m_Rect.Set(rc.left, rc.top, rc.right, rc.bottom);
		break;
		}
	case WM_DESTROY:
		{
		if(m_IconBig)
			DestroyIcon(m_IconBig);
		if(m_IconSmall)
			DestroyIcon(m_IconSmall);
		if(m_Theme)
			m_Theme->Changed.Remove(this);
		SetPropA(m_Handle, "WindowHandle", NULL);
		m_Handle=NULL;
		return 0;
		}
	case WM_DPICHANGED:
		{
		UINT dpi=GetDpiForWindow(m_Handle);
		Scale=(FLOAT)dpi/96;
		Invalidate(true);
		break;
		}
	case WM_ERASEBKGND:
		{
		break;
		}
	case WM_HOTKEY:
		{
		VirtualKey key=(VirtualKey)LOWORD(wparam);
		auto args=KeyEventArgs::Create(key);
		if(DoKey(KeyEventType::KeyDown, args))
			return 0;
		break;
		}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
		VirtualKey key=(VirtualKey)LOWORD(wparam);
		auto args=KeyEventArgs::Create(key);
		if(DoKey(KeyEventType::KeyDown, args))
			return 0;
		break;
		}
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
		VirtualKey key=(VirtualKey)LOWORD(wparam);
		auto args=KeyEventArgs::Create(key);
		if(DoKey(KeyEventType::KeyUp, args))
			return 0;
		break;
		}
	case WM_LBUTTONDOWN:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		auto args=PointerEventArgs::Create(PointerButton::Left, pt);
		if(DoPointer(PointerEventType::ButtonDown, args))
			return 0;
		break;
		}
	case WM_LBUTTONUP:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		auto args=PointerEventArgs::Create(PointerButton::Left, pt);
		if(DoPointer(PointerEventType::ButtonUp, args))
			return 0;
		break;
		}
	case WM_MBUTTONDOWN:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		auto args=PointerEventArgs::Create(PointerButton::Wheel, pt);
		if(DoPointer(PointerEventType::ButtonDown, args))
			return 0;
		break;
		}
	case WM_MBUTTONUP:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		auto args=PointerEventArgs::Create(PointerButton::Wheel, pt);
		if(DoPointer(PointerEventType::ButtonUp, args))
			return 0;
		break;
		}
	case WM_MOUSEMOVE:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		auto args=PointerEventArgs::Create(PointerButton::None, pt);
		if(DoPointer(PointerEventType::Move, args))
			return 0;
		break;
		}
	case WM_MOUSEWHEEL:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		INT delta=GET_WHEEL_DELTA_WPARAM(wparam);
		::RECT rc_screen;
		::GetWindowRect(m_Handle, &rc_screen);
		x-=rc_screen.left;
		y-=rc_screen.top;
		POINT pt(x, y);
		auto args=PointerEventArgs::Create(PointerButton::Wheel, pt, delta);
		if(DoPointer(PointerEventType::Wheel, args))
			return 0;
		break;
		}
	case WM_MOVE:
		{
		m_Rect.SetPosition(LOWORD(lparam), HIWORD(lparam));
		break;
		}
	case WM_PAINT:
		{
		::RECT rc_wnd;
		::GetWindowRect(m_Handle, &rc_wnd);
		RECT rc(rc_wnd.left, rc_wnd.top, rc_wnd.right, rc_wnd.bottom);
		Frame::Move(m_RenderTarget, rc);
		:: RECT rc_client;
		::GetClientRect(m_Handle, &rc_client);
		rc.Set(0, 0, rc_client.right, rc_client.bottom);
		PAINTSTRUCT ps;
		BeginPaint(m_Handle, &ps);
		m_RenderTarget->BeginDraw(ps.hdc, rc);
		RenderWindow(this, m_RenderTarget, rc, false);
		m_RenderTarget->EndDraw();
		EndPaint(m_Handle, &ps);
		break;
		}
	case WM_RBUTTONDOWN:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		auto args=PointerEventArgs::Create(PointerButton::Right, pt);
		if(DoPointer(PointerEventType::ButtonDown, args))
			return 0;
		break;
		}
	case WM_RBUTTONUP:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		auto args=PointerEventArgs::Create(PointerButton::Right, pt);
		if(DoPointer(PointerEventType::ButtonUp, args))
			return 0;
		break;
		}
	case WM_SIZE:
		{
		UINT width=LOWORD(lparam);
		UINT height=HIWORD(lparam);
		m_Rect.SetSize(width, height);
		break;
		}
	case WM_SIZING:
		{
		auto rc_ptr=(::RECT*)lparam;
		::RECT& rc=*rc_ptr;
		SIZE size;
		size.Width=rc.right-rc.left;
		size.Height=rc.bottom-rc.top;
		SIZE min_size=GetMinSize(m_RenderTarget);
		size=size.Max(min_size);
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
		break;
		}
	}
return DefWindowProc(m_Handle, msg, wparam, lparam);
}

UINT Overlapped::Release()noexcept
{
if(m_ReferenceCount==1)
	{
	if(GetActiveWindow()==m_Handle)
		{
		if(m_Parent)
			m_Parent->Activate(FocusReason::None);
		}
	SetPropA(m_Handle, "WindowHandle", NULL);
	DestroyWindow(m_Handle);
	}
return Object::Release();
}


//================
// Common Private
//================

VOID Overlapped::OnIconChanged(Handle<Graphics::Icon> icon)
{
if(m_IconBig)
	{
	DestroyIcon(m_IconBig);
	m_IconBig=NULL;
	}
if(m_IconSmall)
	{
	DestroyIcon(m_IconSmall);
	m_IconSmall=NULL;
	}
LONG style=0;
if(m_Handle!=NULL)
	style=GetWindowLong(m_Handle, GWL_STYLE);
if(icon)
	{
	UINT size_small=GetSystemMetrics(SM_CXSMICON);
	UINT size_big=GetSystemMetrics(SM_CXICON);
	auto bmp_small=icon->GetBitmap(size_small);
	auto bmp_big=icon->GetBitmap(size_big);
	m_IconSmall=CreateIcon(NULL, bmp_small->GetWidth(), bmp_small->GetHeight(), 1, 32, nullptr, bmp_small->Begin());
	m_IconBig=CreateIcon(NULL, bmp_big->GetWidth(), bmp_big->GetHeight(), 1, 32, nullptr, bmp_big->Begin());
	FlagHelper::Set(style, WS_SYSMENU);
	}
else
	{
	FlagHelper::Clear(style, WS_SYSMENU);
	}
if(m_Handle!=NULL)
	{
	SetWindowLong(m_Handle, GWL_STYLE, style);
	SendMessage(m_Handle, WM_SETICON, ICON_SMALL, (LPARAM)m_IconSmall);
	SendMessage(m_Handle, WM_SETICON, ICON_BIG, (LPARAM)m_IconBig);
	}
}

VOID Overlapped::OnInvalidated()
{
DispatchedQueue::Append(this, &Overlapped::Repaint);
}

VOID Overlapped::OnThemeChanged()
{
if(m_Handle==NULL)
	return;
auto scheme=m_Theme->GetColorScheme();
BOOL dark=(scheme==ColorScheme::Dark);
DwmSetWindowAttribute(m_Handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(BOOL));
Invalidate(true);
}

VOID Overlapped::OnTitleChanged(Handle<Sentence> title)
{
if(m_Handle==NULL)
	return;
LONG style=GetWindowLong(m_Handle, GWL_STYLE);
if(title)
	{
	FlagHelper::Set(style, WS_CAPTION);
	SetWindowText(m_Handle, title->Begin());
	}
else
	{
	FlagHelper::Clear(style, WS_CAPTION);
	SetWindowText(m_Handle, TEXT(""));
	}
SetWindowLong(m_Handle, GWL_STYLE, style);
}

VOID Overlapped::OnVisibleChanged(BOOL visible)
{
Show(visible? SW_SHOW: SW_HIDE);
}

LRESULT CALLBACK Overlapped::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
auto window=(Overlapped*)GetPropA(hwnd, "WindowHandle");
if(msg==WM_CREATE)
	{
	CREATESTRUCT* cs=(CREATESTRUCT*)lparam;
	window=(Overlapped*)cs->lpCreateParams;
	SetPropA(hwnd, "WindowHandle", window);
	window->m_Handle=hwnd;
	}
if(window)
	return window->HandleMessage(msg, wparam, lparam);
return DefWindowProc(hwnd, msg, wparam, lparam);
}

}