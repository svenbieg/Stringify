//===========
// Frame.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include <windowsx.h>
#include "Core/Application.h"
#include "Graphics/Direct2D/Theme.h"
#include "UI/Input/KeyEvent.h"
#include "UI/Input/PointerEvent.h"
#include "UI/Input/Shortcut.h"
#include "Frame.h"

using namespace Core;
using namespace Graphics::Direct2D;
using namespace UI;
using namespace UI::Input;


//===========
// Namespace
//===========

namespace Desktop {


//==================
// Con-/Destructors
//==================

Frame::Frame(Window* parent):
Window(parent),
uFlags(FrameFlags::None)
{
Created.Add(this, &Frame::OnCreated);
Message.Add(this, &Frame::OnMessage);
hRenderTarget=new RenderTarget();
auto theme=Theme::Get();
UIFrame=new UI::Frame(hRenderTarget, theme);
UIFrame->CursorChanged.Add(this, &Frame::OnFrameCursorChanged);
UIFrame->Invalidated.Add(this, &Frame::OnFrameInvalidated);
UIFrame->PointerCaptured.Add(this, &Frame::OnFramePointerCaptured);
UIFrame->PointerReleased.Add(this, &Frame::OnFramePointerReleased);
}


//================
// Common Private
//================

VOID Frame::DoRepaint()
{
InvalidateRect(hWindow, nullptr, false);
}

VOID Frame::OnCreated()
{
UpdateDpi();
}

VOID Frame::OnFrameCursorChanged(Handle<Cursor> cursor)
{
auto d2d_cursor=Convert<Graphics::Direct2D::Cursor>(cursor);
if(!d2d_cursor)
	{
	auto theme=Theme::Get();
	d2d_cursor=theme->DefaultCursor;
	}
HCURSOR hcursor=d2d_cursor->GetHandle();
SetClassLongPtr(hWindow, GCLP_HCURSOR, (LONG_PTR)hcursor);
}

VOID Frame::OnFrameInvalidated()
{
Application::Current->Dispatch(this, &Frame::DoRepaint);
}

VOID Frame::OnFramePointerCaptured()
{
SetCapture(hWindow);
}

VOID Frame::OnFramePointerReleased()
{
ReleaseCapture();
}

VOID Frame::OnMessage(WNDMSG& msg)
{
switch(msg.Message)
	{
	case WM_CHAR:
		{
		TCHAR c=(TCHAR)msg.WParam;
		if(c<' ')
			break;
		VirtualKey key=VirtualKey::None;
		Handle<KeyEventArgs> args=new KeyEventArgs(key, c);
		UIFrame->DoKey(KeyEventType::KeyDown, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_DPICHANGED:
		{
		UpdateDpi();
		break;
		}
	case WM_ERASEBKGND:
		{
		msg.Handled=true;
		msg.Status=0;
		break;
		}
	case WM_HOTKEY:
		{
		VirtualKey key=(VirtualKey)LOWORD(msg.WParam);
		Handle<KeyEventArgs> args=new KeyEventArgs(key);
		UIFrame->DoKey(KeyEventType::KeyDown, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
		VirtualKey key=(VirtualKey)LOWORD(msg.WParam);
		Handle<KeyEventArgs> args=new KeyEventArgs(key);
		UIFrame->DoKey(KeyEventType::KeyDown, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
		VirtualKey key=(VirtualKey)LOWORD(msg.WParam);
		Handle<KeyEventArgs> args=new KeyEventArgs(key);
		UIFrame->DoKey(KeyEventType::KeyUp, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_KILLFOCUS:
		{
		UIFrame->KillFocus();
		break;
		}
	case WM_LBUTTONDOWN:
		{
		INT x=GET_X_LPARAM(msg.LParam);
		INT y=GET_Y_LPARAM(msg.LParam);
		Graphics::POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Left, pt);
		UIFrame->DoPointer(PointerEventType::ButtonDown, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_LBUTTONUP:
		{
		INT x=GET_X_LPARAM(msg.LParam);
		INT y=GET_Y_LPARAM(msg.LParam);
		Graphics::POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Left, pt);
		UIFrame->DoPointer(PointerEventType::ButtonUp, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_MBUTTONDOWN:
		{
		INT x=GET_X_LPARAM(msg.LParam);
		INT y=GET_Y_LPARAM(msg.LParam);
		Graphics::POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Wheel, pt);
		UIFrame->DoPointer(PointerEventType::ButtonDown, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_MBUTTONUP:
		{
		INT x=GET_X_LPARAM(msg.LParam);
		INT y=GET_Y_LPARAM(msg.LParam);
		Graphics::POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Wheel, pt);
		UIFrame->DoPointer(PointerEventType::ButtonUp, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_MOUSEMOVE:
		{
		INT x=GET_X_LPARAM(msg.LParam);
		INT y=GET_Y_LPARAM(msg.LParam);
		Graphics::POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::None, pt);
		UIFrame->DoPointer(PointerEventType::Move, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_MOUSEWHEEL:
		{
		INT x=GET_X_LPARAM(msg.LParam);
		INT y=GET_Y_LPARAM(msg.LParam);
		INT delta=GET_WHEEL_DELTA_WPARAM(msg.WParam);
		::RECT rc_screen;
		::GetWindowRect(hWindow, &rc_screen);
		x-=rc_screen.left;
		y-=rc_screen.top;
		Graphics::POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Wheel, pt, delta);
		UIFrame->DoPointer(PointerEventType::Wheel, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint(hWindow, &ps);
		RECT rc=GetClientRect();
		hRenderTarget->BeginDraw(ps.hdc, rc);
		UIFrame->Move(hRenderTarget, rc);
		UIFrame->Render(hRenderTarget);
		hRenderTarget->EndDraw();
		EndPaint(hWindow, &ps);
		msg.Handled=true;
		break;
		}
	case WM_RBUTTONDOWN:
		{
		INT x=GET_X_LPARAM(msg.LParam);
		INT y=GET_Y_LPARAM(msg.LParam);
		POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Right, pt);
		UIFrame->DoPointer(PointerEventType::ButtonDown, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_RBUTTONUP:
		{
		INT x=GET_X_LPARAM(msg.LParam);
		INT y=GET_Y_LPARAM(msg.LParam);
		Graphics::POINT pt(x, y);
		Handle<PointerEventArgs> args=new PointerEventArgs(PointerButton::Right, pt);
		UIFrame->DoPointer(PointerEventType::ButtonUp, args);
		msg.Handled=args->Handled;
		break;
		}
	case WM_SIZE:
		{
		Repaint();
		break;
		}
	}
}

VOID Frame::UpdateDpi()
{
UIFrame->Scale=GetScaleFactor();
UIFrame->Invalidate(true);
}

}