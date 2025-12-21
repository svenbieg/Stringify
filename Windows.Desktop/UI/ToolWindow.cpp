//================
// ToolWindow.cpp
//================

#include "ToolWindow.h"


//===========
// Namespace
//===========

namespace UI {


//============================
// Con-/Destructors Protected
//============================

ToolWindow::ToolWindow():
Title(this)
{
Title.Changed.Add(this, &ToolWindow::OnTitleChanged);
SetWindowLong(m_Handle, GWL_STYLE, WS_CAPTION|WS_OVERLAPPED);
SetWindowLong(m_Handle, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
}


//================
// Common Private
//================

VOID ToolWindow::OnTitleChanged(Handle<Sentence> title)
{
SetWindowText(m_Handle, title? title->Begin(): TEXT(""));
}

}