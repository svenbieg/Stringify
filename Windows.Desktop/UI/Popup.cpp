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

Popup::Popup(Window* content)
{
SetWindowLong(m_Handle, GWL_STYLE, WS_DLGFRAME);
SetWindowLong(m_Handle, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
content->SetParent(this);
}

}