//================
// Overlapped.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include <windowsx.h>
#include "Core/Application.h"
#include "Graphics/Direct2D/Theme.h"
#include "Overlapped.h"

using namespace Core;

using D2DCursor=Graphics::Direct2D::Cursor;
using D2DTheme=Graphics::Direct2D::Theme;


//===========
// Namespace
//===========

namespace UI {


//========
// Common
//========

VOID Overlapped::BringToFront()
{
SetWindowPos(m_Handle, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
Frame::BringToFront();
}

Graphics::RECT Overlapped::GetBorderWidth()const
{
if(!m_Handle)
	return RECT();
::RECT rc({ 0, 0, 0, 0});
UINT style=GetWindowLongA(m_Handle, GWL_STYLE);
UINT style_ex=GetWindowLongA(m_Handle, GWL_EXSTYLE);
AdjustWindowRectEx(&rc, style, false, style_ex);
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

Graphics::POINT Overlapped::GetScreenOffset()const
{
POINT pt=Frame::GetScreenOffset();
RECT border=GetBorderWidth();
pt.Left+=border.Left;
pt.Top+=border.Top;
return pt;
}

Handle<Graphics::Theme> Overlapped::GetTheme()
{
return D2DTheme::Get();
}

VOID Overlapped::Move(RECT const& rc)
{
if(!m_Handle)
	return;
SIZE size=GetMinSize(m_RenderTarget);
size.Max(rc.GetSize());
MoveWindow(m_Handle, rc.Left, rc.Top, size.Width, size.Height, true);
}

VOID Overlapped::Repaint()
{
if(m_Handle)
	{
	InvalidateRect(m_Handle, nullptr, false);
	UpdateWindow(m_Handle);
	}
}

VOID Overlapped::SetCursor(Handle<Cursor> cursor)
{
HCURSOR hcursor=m_Cursor;
auto d2d_cursor=cursor.As<D2DCursor>();
if(d2d_cursor)
	hcursor=d2d_cursor->GetHandle();
SetClassLongPtr(m_Handle, GCLP_HCURSOR, (LONG_PTR)hcursor);
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
Visible.Set(visible, false);
ShowWindow(m_Handle, show);
}

VOID Overlapped::Shrink()
{
if(!m_Handle)
	return;
SIZE size=GetMinSize(m_RenderTarget);
RECT rc(m_Rect);
rc.SetSize(size);
Move(rc);
}


//============================
// Con-/Destructors Protected
//============================

Overlapped::Overlapped():
Overlapped(HWND_DESKTOP)
{}

Overlapped::Overlapped(HWND parent):
m_Cursor(NULL),
m_Handle(NULL)
{
m_Cursor=LoadCursor(NULL, IDC_ARROW);
auto theme=GetTheme();
Background=theme->ControlBrush;
Invalidated.Add(this, &Overlapped::OnInvalidated);
Visible.Changed.Add(this, &Overlapped::OnVisibleChanged);
Visible.Set(false, false);
m_RenderTarget=new D2DRenderTarget();
LPCTSTR class_name=TEXT("Overlapped");
HINSTANCE inst=GetModuleHandle(nullptr);
WNDCLASSEX wc;
ZeroMemory(&wc, sizeof(WNDCLASSEX));
wc.cbSize=sizeof(WNDCLASSEX);
wc.hCursor=m_Cursor;
wc.hInstance=inst;
wc.lpfnWndProc=&WndProc;
wc.lpszClassName=class_name;
wc.style=CS_HREDRAW|CS_VREDRAW;
SetLastError(0);
RegisterClassEx(&wc);
UINT style=WS_OVERLAPPED;
m_Handle=CreateWindowEx(0, class_name, nullptr, style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, NULL, inst, this);
if(m_Handle==INVALID_HANDLE_VALUE)
	m_Handle=NULL;
}

Overlapped::Overlapped(Window* parent):
Overlapped(parent? parent->GetFrame(): nullptr)
{}

Overlapped::Overlapped(Frame* parent):
Overlapped(Convert<Overlapped>(parent))
{}

Overlapped::Overlapped(Overlapped* parent):
Overlapped(parent? parent->GetHandle(): HWND_DESKTOP)
{}


//================
// Common Private
//================

LRESULT Overlapped::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam, BOOL& handled)
{
switch(msg)
	{
	case WM_CHAR:
		{
		TCHAR c=(TCHAR)wparam;
		if(c<' ')
			break;
		VirtualKey key=VirtualKey::None;
		Handle<KeyEventArgs> args=new KeyEventArgs(key, c);
		DoKey(KeyEventType::KeyDown, args);
		handled=args->Handled;
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
		m_Handle=NULL;
		break;
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
		Handle<KeyEventArgs> args=new KeyEventArgs(key);
		DoKey(KeyEventType::KeyDown, args);
		handled=args->Handled;
		break;
		}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
		VirtualKey key=(VirtualKey)LOWORD(wparam);
		Handle<KeyEventArgs> args=new KeyEventArgs(key);
		DoKey(KeyEventType::KeyDown, args);
		handled=args->Handled;
		break;
		}
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
		VirtualKey key=(VirtualKey)LOWORD(wparam);
		Handle<KeyEventArgs> args=new KeyEventArgs(key);
		DoKey(KeyEventType::KeyUp, args);
		handled=args->Handled;
		break;
		}
	case WM_KILLFOCUS:
		{
		this->KillFocus();
		break;
		}
	case WM_LBUTTONDOWN:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Left, pt);
		DoPointer(PointerEventType::ButtonDown, args);
		handled=args->Handled;
		break;
		}
	case WM_LBUTTONUP:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Left, pt);
		DoPointer(PointerEventType::ButtonUp, args);
		handled=args->Handled;
		break;
		}
	case WM_MBUTTONDOWN:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Wheel, pt);
		DoPointer(PointerEventType::ButtonDown, args);
		handled=args->Handled;
		break;
		}
	case WM_MBUTTONUP:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Wheel, pt);
		DoPointer(PointerEventType::ButtonUp, args);
		handled=args->Handled;
		break;
		}
	case WM_MOUSEMOVE:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::None, pt);
		DoPointer(PointerEventType::Move, args);
		handled=args->Handled;
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
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Wheel, pt, delta);
		DoPointer(PointerEventType::Wheel, args);
		handled=args->Handled;
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
		auto target=m_RenderTarget.As<D2DRenderTarget>();
		PAINTSTRUCT ps;
		BeginPaint(m_Handle, &ps);
		target->BeginDraw(ps.hdc, rc);
		RenderWindow(this, target, rc, false);
		target->EndDraw();
		EndPaint(m_Handle, &ps);
		break;
		}
	case WM_RBUTTONDOWN:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Right, pt);
		DoPointer(PointerEventType::ButtonDown, args);
		handled=args->Handled;
		break;
		}
	case WM_RBUTTONUP:
		{
		INT x=GET_X_LPARAM(lparam);
		INT y=GET_Y_LPARAM(lparam);
		POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Right, pt);
		DoPointer(PointerEventType::ButtonUp, args);
		handled=args->Handled;
		break;
		}
	case WM_SHOWWINDOW:
		{
		handled=false;
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
	default:
		{
		handled=false;
		break;
		}
	}
return 0;
}

VOID Overlapped::OnInvalidated()
{
Application::Current->Dispatch(this, &Overlapped::Repaint);
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
if(!window)
	return DefWindowProc(hwnd, msg, wparam, lparam);
BOOL handled=true;
LRESULT lr=window->HandleMessage(msg, wparam, lparam, handled);
if(handled)
	return lr;
return DefWindowProc(hwnd, msg, wparam, lparam);
}

}