//================
// ToolWindow.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "ToolWindow.h"


//===========
// Namespace
//===========

namespace UI {


//============================
// Con-/Destructors Protected
//============================

ToolWindow::ToolWindow():
ToolWindow(HWND_DESKTOP)
{}

ToolWindow::ToolWindow(HWND parent):
Overlapped(parent),
Title(this)
{
Title.Changed.Add(this, &ToolWindow::OnTitleChanged);
SetWindowLong(m_Handle, GWL_STYLE, WS_CAPTION|WS_OVERLAPPED);
SetWindowLong(m_Handle, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
}

ToolWindow::ToolWindow(Window* parent):
ToolWindow(parent? parent->GetFrame(): nullptr)
{}

ToolWindow::ToolWindow(Frame* parent):
ToolWindow(Convert<Overlapped>(parent))
{}

ToolWindow::ToolWindow(Overlapped* parent):
ToolWindow(parent? parent->GetHandle(): HWND_DESKTOP)
{}


//================
// Common Private
//================

VOID ToolWindow::OnTitleChanged(Handle<Sentence> title)
{
SetWindowTextA(m_Handle, title? title->Begin(): "");
}

}