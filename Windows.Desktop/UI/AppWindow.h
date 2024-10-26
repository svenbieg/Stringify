//=============
// AppWindow.h
//=============

#pragma once


//=======
// Using
//=======

#include "Graphics/Icon.h"
#include "UI/Controls/StackPanel.h"
#include "UI/Overlapped.h"


//===========
// Namespace
//===========

namespace UI {


//============
// App-Window
//============

class AppWindow: public Overlapped
{
public:
	// Using
	using Panel=UI::Controls::Panel;
	using StackPanel=UI::Controls::StackPanel;

	// Common
	VOID Activate();
	Event<AppWindow> Activated;
	Handle<Panel> Body;
	VOID Close();
	Event<AppWindow> Closed;
	static Handle<AppWindow> Current;
	Handle<StackPanel> Footer;
	Handle<StackPanel> Header;
	DynamicHandle<AppWindow, Graphics::Icon> Icon;
	Event<AppWindow> Maximized;
	Event<AppWindow> Minimized;
	DynamicHandle<AppWindow, Sentence> Title;

protected:
	// Con-/Destructors
	AppWindow();

	// Common
	LRESULT HandleMessage(UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled)override;

private:
	// Common
	VOID OnClosed();
	VOID OnIconChanged(Handle<Graphics::Icon> Icon);
	VOID OnTitleChanged(Handle<Sentence> Title);
};

}