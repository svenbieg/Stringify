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
	using Sentence=Culture::Sentence;
	using StackPanel=UI::Controls::StackPanel;

	// Con-/Destructors
	~AppWindow();

	// Common
	Handle<Panel> Body;
	Event<AppWindow> Closed;
	Handle<StackPanel> Footer;
	static inline AppWindow* GetCurrent() { return s_Current; }
	Handle<StackPanel> Header;
	Event<AppWindow> Maximized;
	Event<AppWindow> Minimized;

protected:
	// Con-/Destructors
	AppWindow();

	// Common
	LRESULT HandleMessage(UINT Message, WPARAM WParam, LPARAM LParam)override;

private:
	// Common
	VOID OnClosed();
	VOID OnSettingChanged(LPCTSTR Setting);
	static AppWindow* s_Current;
};

}