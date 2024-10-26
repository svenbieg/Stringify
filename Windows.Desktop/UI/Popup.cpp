//===========
// Popup.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Popup.h"


//===========
// Namespace
//===========

namespace UI {


//============================
// Con-/Destructors Protected
//============================

Popup::Popup(Window* parent):
Overlapped(Convert<Overlapped>(parent->GetFrame()))
{
SetWindowLong(m_Handle, GWL_STYLE, WS_DLGFRAME);
SetWindowLong(m_Handle, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
}


//==================
// Common Protected
//==================

LRESULT Popup::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam, BOOL& handled)
{
switch(msg)
	{
	case WM_KILLFOCUS:
		{
		KillFocus();
		Visible=false;
		return 0;
		}
	}
return Overlapped::HandleMessage(msg, wparam, lparam, handled);
}

}