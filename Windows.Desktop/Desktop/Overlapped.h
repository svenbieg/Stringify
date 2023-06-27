//==============
// Overlapped.h
//==============

#pragma once


//=======
// Using
//=======

#include "Graphics/Direct2D/Icon.h"
#include "Resources/Icons/Icon.h"
#include "Frame.h"


//===========
// Namespace
//===========

namespace Desktop {


//============
// Overlapped
//============

class Overlapped: public Frame
{
public:
	// Using
	using D2DIcon=Graphics::Direct2D::Icon;

	// Con-/Destructors
	Overlapped(Window* Parent, Handle<D2DIcon> Icon=nullptr);

	// Common
	VOID Activate();
	Event<Window> Activated;
	VOID Center();
	Property<Overlapped, BOOL> CloseButton;
	Event<Window> Closed;
	VOID Create(LPCTSTR Class, UINT Style, UINT StyleEx=0);
	RECT GetBorderWidth();
	SIZE GetMinSize()override;
	Event<Window> Maximized;
	Event<Window> Minimized;
	VOID MinimizeSize();
	DynamicHandle<Overlapped, Sentence> Title;

private:
	// Common
	VOID OnCloseButtonChanged(BOOL Button);
	VOID OnCreated();
	VOID OnMessage(WNDMSG& Message);
	VOID OnTitleChanged(Handle<Sentence> Title);
	Handle<D2DIcon> hIcon;
};

}