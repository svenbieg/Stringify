//=============
// AppWindow.h
//=============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/StackPanel.h"
#include "Overlapped.h"


//===========
// Namespace
//===========

namespace Desktop {


//=======
// Style
//=======

enum class AppWindowStyle
{
Default,
Dialog,
DialogWindow
};


//============
// App-Window
//============

class AppWindow: public Overlapped
{
public:
	// Using
	using AppWindowStyle=Desktop::AppWindowStyle;
	using Panel=UI::Controls::Panel;
	using StackPanel=UI::Controls::StackPanel;

	// Con-/Destructors
	AppWindow(Handle<D2DIcon> Icon, AppWindowStyle Style=AppWindowStyle::Default);
	~AppWindow();

	// Common
	VOID Close();
	VOID Create();
	Handle<Panel> Body;
	static AppWindow* Current;
	Handle<StackPanel> Footer;
	Handle<StackPanel> Header;
	VOID Show(INT ShowCommand);

private:
	// Common
	VOID OnCreated();
	VOID OnDestroyed();
	AppWindowStyle uStyle;
};

}