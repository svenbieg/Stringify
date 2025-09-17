//=============
// AppWindow.h
//=============

#pragma once


//=======
// Using
//=======

#include "Culture/Sentence.h"
#include "Graphics/Icon.h"
#include "UI/Controls/StackPanel.h"
#include "UI/Overlapped.h"
#include "DynamicHandle.h"


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
	static inline AppWindow* Get() { return s_Current; }

	// Common
	VOID Activate();
	Event<AppWindow> Activated;
	Handle<Panel> Body;
	VOID Close();
	Event<AppWindow> Closed;
	Handle<StackPanel> Footer;
	Handle<StackPanel> Header;
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
	VOID OnSettingChanged(LPCTSTR Setting);
	VOID OnTitleChanged(Handle<Sentence> Title);
	static AppWindow* s_Current;
};

}