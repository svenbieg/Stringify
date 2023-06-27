//=================
// Application.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Clipboard.h"
#include "Application.h"

using namespace Storage;
using namespace UI::Controls;


//===========
// Namespace
//===========

namespace Framework {


//========
// Common
//========

Application* Application::Current=nullptr;

VOID Application::EditCopy()
{
auto input=GetInputControl();
if(!input)
	return;
auto clipboard=Clipboard::Open();
auto text=input->GetSelection();
clipboard->Copy(text);
}

VOID Application::EditCut()
{
auto input=GetInputControl();
if(!input)
	return;
auto clipboard=Clipboard::Open();
auto text=input->GetSelection();
clipboard->Copy(text);
input->ReplaceSelection(nullptr);
}

VOID Application::EditDelete()
{
auto input=GetInputControl();
if(!input)
	return;
input->ReplaceSelection(nullptr);
}

VOID Application::EditPaste()
{
auto input=GetInputControl();
if(!input)
	return;
auto clipboard=Clipboard::Open();
BOOL has=clipboard->HasText();
auto text=clipboard->GetText();
if(!text)
	return;
input->ReplaceSelection(text->Begin());
}

VOID Application::EditSelectAll()
{
auto input=GetInputControl();
if(!input)
	return;
input->SelectAll();
}


//============================
// Con-/Destructors Protected
//============================

Application::Application(LPCSTR name, LPCSTR version):
Core::Application(name, version),
pFrame(nullptr)
{
Current=this;
}


//==================
// Common Protected
//==================

Input* Application::GetInputControl()
{
if(!pFrame)
	return nullptr;
auto focus=pFrame->GetOldFocus();
if(!focus)
	focus=pFrame->GetFocus();
if(!focus)
	return nullptr;
return Convert<Input>(focus);
}

}