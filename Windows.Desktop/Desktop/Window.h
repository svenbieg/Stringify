//==========
// Window.h
//==========

#pragma once


//=======
// Using
//=======

#include "Collections/List.h"
#include "Graphics/Point.h"
#include "Graphics/Rect.h"
#include "Graphics/Size.h"


//===========
// Namespace
//===========

namespace Desktop {


//=========
// Message
//=========

typedef struct
{
BOOL Handled;
LPARAM LParam;
UINT Message;
LRESULT Status;
WPARAM WParam;
}WNDMSG;


//========
// Window
//========

class Window: public Object
{
public:
	// Using
	using ChildList=Collections::List<Handle<Window>>;
	using POINT=Graphics::POINT;
	using RECT=Graphics::RECT;
	using SIZE=Graphics::SIZE;

	// Con-/Destructors
	Window(Window* Parent);
	virtual ~Window();

	// Common
	VOID BringToFront();
	Handle<ChildList> Children;
	Event<Window> Created;
	VOID Destroy();
	Event<Window> Destroyed;
	RECT GetClientRect()const;
	HWND GetHandle()const { return hWindow; }
	virtual SIZE GetMinSize();
	RECT GetRect()const;
	FLOAT GetScaleFactor()const;
	BOOL GetStyle(UINT Style);
	Handle<Window> GetVisibleChild(UINT Id);
	VOID Invalidate();
	VOID InvalidateSize();
	Event<Window, WNDMSG&> Message;
	SIZE MinSize;
	VOID Move(RECT const& Rect);
	Window* const Parent;
	VOID Repaint();
	VOID SetStyle(UINT Style, BOOL Set=true);
	Event<Window> SizeChanged;
	Property<Window, BOOL> Visible;

protected:
	// Common
	VOID Create(LPCTSTR Class, UINT ClassStyle=0, UINT Style=0, UINT StyleEx=0);
	HWND hWindow;

private:
	// Common
	VOID OnMessage(WNDMSG& Message);
	VOID OnVisibleChanged(BOOL Visible);
	static LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
	static LRESULT CALLBACK WndProcCom(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
	WNDPROC pWndProcCom;
};

}