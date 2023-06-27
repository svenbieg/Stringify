//============
// Dialog.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Dialog.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//==================
// Con-/Destructors
//==================

Dialog::Dialog(Window* parent):
Overlapped(parent)
{}


//========
// Common
//========

VOID Dialog::Create(LPCTSTR class_name)
{
Overlapped::Create(class_name, WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU);
}

}}