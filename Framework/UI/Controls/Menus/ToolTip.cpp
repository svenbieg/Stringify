//=============
// ToolTip.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "UI/Controls/TextBlock.h"
#include "UI/Frame.h"
#include "ToolTip.h"

using namespace Concurrency;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==================
// Con-/Destructors
//==================

ToolTip::~ToolTip()
{
if(m_Parent)
	{
	m_Parent->PointerEntered.Remove(this);
	m_Parent->PointerLeft.Remove(this);
	m_Parent=nullptr;
	}
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
	DispatchedQueue::Append(this, &ToolTip::DoClose);
	}
}


//==========================
// Con-/Destructors Private
//==========================

ToolTip::ToolTip(Interactive* parent, Handle<Sentence> text):
Text(this, text),
m_Parent(parent)
{
Text.Changed.Add(this, &ToolTip::OnTextChanged);
OnTextChanged();
}


//================
// Common Private
//================

VOID ToolTip::DoClose()
{
if(m_Panel)
	{
	auto frame=m_Parent->GetFrame();
	frame->Children->Remove(m_Panel);
	m_Panel=nullptr;
	}
}

VOID ToolTip::OnParentClicked()
{
Close();
}

VOID ToolTip::OnParentPointerEntered()
{
m_Timer=Timer::Create();
m_Timer->Triggered.Add(this, &ToolTip::OnTimerTriggered);
m_Timer->StartOnce(1000);
}

VOID ToolTip::OnParentPointerLeft()
{
Close();
}

VOID ToolTip::OnTextChanged()
{
m_Parent->Clicked.Remove(this);
m_Parent->PointerEntered.Remove(this);
m_Parent->PointerLeft.Remove(this);
if(Text)
	{
	m_Parent->Clicked.Add(this, &ToolTip::OnParentClicked);
	m_Parent->PointerEntered.Add(this, &ToolTip::OnParentPointerEntered);
	m_Parent->PointerLeft.Add(this, &ToolTip::OnParentPointerLeft);
	}
if(m_Panel)
	OnTimerTriggered();
}

VOID ToolTip::OnTimerTriggered()
{
if(m_Panel)
	return;
auto frame=m_Parent->GetFrame();
auto theme=m_Parent->GetTheme();
m_Timer=nullptr;
m_Panel=Panel::Create(frame);
m_Panel->Background=theme->WindowBrush;
m_Panel->Border=true;
auto text_block=TextBlock::Create(m_Panel);
text_block->Margin.Set(4, 2, 4, 2);
text_block->Text=Text->Begin();
auto rc_control=m_Parent->GetFrameRect();
Graphics::POINT pt(rc_control.Left, rc_control.Bottom);
m_Panel->SetPosition(pt);
}

}}}