//===============
// AppWindow.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "UI/Controls/Grid.h"
#include "Desktop/Application.h"
#include "AppWindow.h"

using namespace UI;
using namespace UI::Controls;


//===========
// Namespace
//===========

namespace Desktop {


//==================
// Con-/Destructors
//==================

AppWindow::AppWindow(Handle<D2DIcon> icon, AppWindowStyle style):
Overlapped(nullptr, icon),
uStyle(style)
{
Current=this;
Created.Add(this, &AppWindow::OnCreated);
Destroyed.Add(this, &AppWindow::OnDestroyed);
auto grid=new Grid(UIFrame);
grid->AddRow(1, GridUnit::Auto);
grid->AddRow(1, GridUnit::Star);
grid->AddRow(1, GridUnit::Auto);
auto theme=UIFrame->GetTheme();
Header=new StackPanel(grid, Orientation::Vertical);
Header->AlignChildren=Alignment::Stretch;
Header->Background=theme->GetControlBrush();
Body=new Panel(grid);
Body->Background=theme->GetControlBrush();
Footer=new StackPanel(grid, Orientation::Vertical);
Footer->AlignChildren=Alignment::Stretch;
Footer->Background=theme->GetControlBrush();
}

AppWindow::~AppWindow()
{
if(Current==this)
	Current=nullptr;
}


//========
// Common
//========

VOID AppWindow::Close()
{
SendMessage(hWindow, WM_CLOSE, 0, 0);
}

VOID AppWindow::Create()
{
UINT style=WS_OVERLAPPEDWINDOW;
if(uStyle==AppWindowStyle::Dialog)
	style=WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU;
Overlapped::Create(TEXT("AppWindow"), style, WS_EX_APPWINDOW);
}

AppWindow* AppWindow::Current=nullptr;

VOID AppWindow::Show(INT show_cmd)
{
ShowWindow(hWindow, show_cmd);
}


//================
// Common Private
//================

VOID AppWindow::OnCreated()
{
if(!Title)
	Title=Application::Current->Name;
BOOL dialog=(uStyle==AppWindowStyle::Dialog);
dialog|=(uStyle==AppWindowStyle::DialogWindow);
if(dialog)
	{
	MinimizeSize();
	Center();
	}
}

VOID AppWindow::OnDestroyed()
{
Application::Current->Exit();
}

}