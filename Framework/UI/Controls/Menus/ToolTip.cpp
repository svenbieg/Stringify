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
pControl(control)
{
Text.Changed.Add(this, &ToolTip::OnTextChanged);
OnTextChanged();
}

ToolTip::~ToolTip()
{
pControl->PointerEntered.Remove(this);
pControl->PointerLeft.Remove(this);
}


//========
// Common
//========

VOID ToolTip::Close()
{
m_Timer=nullptr;
if(hPanel)
	{
	hPanel->Visible=false;
	Application::Current->Dispatch(this, &ToolTip::DoClose);
	}
}


//================
// Common Private
//================

VOID ToolTip::DoClose()
{
hPanel->Parent=nullptr;
hPanel=nullptr;
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
pControl->Clicked.Remove(this);
pControl->PointerEntered.Remove(this);
pControl->PointerLeft.Remove(this);
if(Text)
	{
	pControl->Clicked.Add(this, &ToolTip::OnControlClicked);
	pControl->PointerEntered.Add(this, &ToolTip::OnControlPointerEntered);
	pControl->PointerLeft.Add(this, &ToolTip::OnControlPointerLeft);
	}
if(hPanel)
	OnTimerTriggered();
}

VOID ToolTip::OnTimerTriggered()
{
if(hPanel)
	return;
m_Timer=nullptr;
auto frame=pControl->GetFrame();
auto theme=frame->GetTheme();
hPanel=new Panel(frame);
hPanel->Background=theme->WindowBrush;
hPanel->Border=true;
auto text_block=new TextBlock(hPanel);
text_block->Margin.Set(4, 2, 4, 2);
text_block->Text=Text->Begin();
auto rc_control=pControl->GetFrameRect();
Graphics::POINT pt(rc_control.Left, rc_control.Bottom);
hPanel->SetPosition(pt);
}

}}}