//=============
// ToolTip.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Core/Application.h"
#include "UI/Controls/TextBlock.h"
#include "UI/Frame.h"
#include "ToolTip.h"

using namespace Core;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

ToolTip::ToolTip(Interactive* control, Handle<Sentence> text):
Text(this, text),
m_Control(control)
{
Text.Changed.Add(this, &ToolTip::OnTextChanged);
OnTextChanged();
}

ToolTip::~ToolTip()
{
m_Control->PointerEntered.Remove(this);
m_Control->PointerLeft.Remove(this);
}


//========
// Common
//========

VOID ToolTip::Close()
{
m_Timer=nullptr;
if(m_Panel)
	{
	m_Panel->Visible=false;
	Application::Current->Dispatch(this, &ToolTip::DoClose);
	}
}


//================
// Common Private
//================

VOID ToolTip::DoClose()
{
m_Panel->Parent=nullptr;
m_Panel=nullptr;
}

VOID ToolTip::OnControlClicked()
{
Close();
}

VOID ToolTip::OnControlPointerEntered()
{
m_Timer=new Timer();
m_Timer->Triggered.Add(this, &ToolTip::OnTimerTriggered);
m_Timer->StartOnce(1000);
}

VOID ToolTip::OnControlPointerLeft()
{
Close();
}

VOID ToolTip::OnTextChanged()
{
m_Control->Clicked.Remove(this);
m_Control->PointerEntered.Remove(this);
m_Control->PointerLeft.Remove(this);
if(Text)
	{
	m_Control->Clicked.Add(this, &ToolTip::OnControlClicked);
	m_Control->PointerEntered.Add(this, &ToolTip::OnControlPointerEntered);
	m_Control->PointerLeft.Add(this, &ToolTip::OnControlPointerLeft);
	}
if(m_Panel)
	OnTimerTriggered();
}

VOID ToolTip::OnTimerTriggered()
{
if(m_Panel)
	return;
m_Timer=nullptr;
auto frame=m_Control->GetFrame();
auto theme=frame->GetTheme();
m_Panel=new Panel(frame);
m_Panel->Background=theme->WindowBrush;
m_Panel->Border=true;
auto text_block=new TextBlock(m_Panel);
text_block->Margin.Set(4, 2, 4, 2);
text_block->Text=Text->Begin();
auto rc_control=m_Control->GetFrameRect();
Graphics::POINT pt(rc_control.Left, rc_control.Bottom);
m_Panel->SetPosition(pt);
}

}}}