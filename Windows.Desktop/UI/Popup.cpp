//===========
// Popup.cpp
//===========

#include "Popup.h"


//===========
// Namespace
//===========

namespace UI {


//============================
// Con-/Destructors Protected
//============================

Popup::Popup(Overlapped* parent):
Overlapped(parent),
Content(this)
{
Content.Changed.Add(this, &Popup::OnContentChanged);
SetWindowLong(m_Handle, GWL_STYLE, WS_DLGFRAME);
SetWindowLong(m_Handle, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
}


//==================
// Common Protected
//==================

UINT Popup::Release()noexcept
{
if(m_ReferenceCount==1)
	Content=nullptr;
return Overlapped::Release();
}


//================
// Common Private
//================

VOID Popup::OnContentChanged(Handle<Window> content)
{
auto child=Children->GetAt(0);
if(child)
	{
	child->SetParent(nullptr);
	Children->Clear(EventNotification::None);
	}
if(content)
	content->SetParent(this);
}

}