//===============
// WrapPanel.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "WrapPanel.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

WrapPanel::WrapPanel(Window* parent):
Panel(parent)
{}


//========
// Common
//========

VOID WrapPanel::Rearrange(RenderTarget* target, RECT& rc)
{
auto border=this->GetBorderBrush();
if(border)
	rc.SetPadding(1, 1, 1, 1);
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
RECT rc_move(rc);
UINT line_height=0;
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	SIZE child_size=child->GetMinSize(target);
	auto control=Convert<Control>(child);
	if(control)
		{
		RECT const& margin=control->Margin;
		child_size.AddPadding(margin*scale);
		}
	line_height=MAX(line_height, child_size.Height);
	rc_move.Right=rc_move.Left+child_size.Width;
	if(rc_move.Right>rc.Right)
		{
		rc_move.Left=0;
		rc_move.Right=child_size.Width;
		rc_move.Top+=line_height;
		line_height=child_size.Height;
		}
	rc_move.Bottom=rc_move.Top+child_size.Height;
	RECT rc_child(rc_move);
	if(control)
		rc_child.SetPadding(control->Margin*scale);
	child->Move(target, rc_child);
	rc_move.Left=rc_move.Right;
	}
}

}}