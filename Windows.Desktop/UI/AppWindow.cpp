//===============
// AppWindow.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Graphics/Direct2D/Icon.h"
#include "UI/Controls/Grid.h"
#include "UI/Application.h"
#include "UI/AppWindow.h"

using namespace UI::Controls;

using D2DIcon=Graphics::Direct2D::Icon;


//===========
// Namespace
//===========

namespace UI {


//==================
// Con-/Destructors
//==================

AppWindow::~AppWindow()
{
s_Current=nullptr;
}


//========
// Common
//========

VOID AppWindow::Activate()
{
WINDOWPLACEMENT place={ 0 };
place.length=sizeof(WINDOWPLACEMENT);
GetWindowPlacement(m_Handle, &place);
if(place.showCmd==SW_SHOWMINIMIZED)
	place.showCmd=SW_RESTORE;
ShowWindow(m_Handle, place.showCmd);
SetWindowPos(m_Handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
SetForegroundWindow(m_Handle);
}

VOID AppWindow::Close()
{
if(m_Handle)
	SendMessage(m_Handle, WM_CLOSE, 0, 0);
}


//============================
// Con-/Destructors Protected
//============================

AppWindow::AppWindow():
Icon(this),
Title(this)
{
s_Current=this;
SetWindowLong(m_Handle, GWL_STYLE, WS_OVERLAPPEDWINDOW);
SetWindowLong(m_Handle, GWL_EXSTYLE, WS_EX_APPWINDOW);
Closed.Add(this, &AppWindow::OnClosed);
Icon.Changed.Add(this, &AppWindow::OnIconChanged);
Title.Changed.Add(this, &AppWindow::OnTitleChanged);
Title=Application::Get()->GetName();
auto grid=Grid::Create(this);
grid->AddRow(1, GridUnit::Auto);
grid->AddRow(1, GridUnit::Star);
grid->AddRow(1, GridUnit::Auto);
Header=StackPanel::Create(grid, Orientation::Vertical);
Header->AlignChildren=Alignment::Stretch;
Body=Panel::Create(grid);
Footer=StackPanel::Create(grid, Orientation::Vertical);
Footer->AlignChildren=Alignment::Stretch;
}


//==================
// Common Protected
//==================

LRESULT AppWindow::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam, BOOL& handled)
{
switch(msg)
	{
	case WM_ACTIVATE:
		{
		Activated(this);
		return 0;
		}
	case WM_CLOSE:
		{
		if(Closed)
			{
			Closed(this);
			}
		else
			{
			DestroyWindow(m_Handle);
			}
		return 0;
		}
	case WM_SIZE:
		{
		switch(wparam)
			{
			case SIZE_MAXIMIZED:
				{
				Maximized(this);
				break;
				}
			case SIZE_MINIMIZED:
				{
				Minimized(this);
				break;
				}
			}
		break;
		}
	}
return Overlapped::HandleMessage(msg, wparam, lparam, handled);
}


//================
// Common Private
//================

VOID AppWindow::OnClosed()
{
Application::Get()->Quit();
}

VOID AppWindow::OnIconChanged(Handle<Graphics::Icon> icon)
{
auto d2d_icon=icon.As<D2DIcon>();
UINT size_small=GetSystemMetrics(SM_CXSMICON);
UINT size_big=GetSystemMetrics(SM_CXICON);
HICON ico_small=d2d_icon->GetHandle(size_small);
HICON ico_big=d2d_icon->GetHandle(size_big);
SendMessage(m_Handle, WM_SETICON, ICON_SMALL, (LPARAM)ico_small);
SendMessage(m_Handle, WM_SETICON, ICON_BIG, (LPARAM)ico_big);
}

VOID AppWindow::OnTitleChanged(Handle<Sentence> title)
{
SetWindowText(m_Handle, title? title->Begin(): TEXT(""));
}

AppWindow* AppWindow::s_Current=nullptr;

}