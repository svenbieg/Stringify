//================
// StackPanel.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "StackPanel.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

StackPanel::StackPanel(Orientation orientation):
StackPanel(nullptr, orientation)
{}

StackPanel::StackPanel(Window* parent, Orientation orientation):
Panel(parent),
AlignChildren(Alignment::Default),
uOrientation(orientation)
{}


//========
// Common
//========

Graphics::SIZE StackPanel::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
FLOAT scale=GetScaleFactor();
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
	if(uOrientation==Orientation::Horizontal)
		{
		size.Width+=child_size.Width;
		size.Height=MAX(size.Height, child_size.Height);
		}
	else
		{
		size.Width=MAX(size.Width, child_size.Width);
		size.Height+=child_size.Height;
		}
	}
auto border=this->GetBorderBrush();
if(border)
	size.Add(2, 2);
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID StackPanel::Rearrange(RenderTarget* target, RECT& rc)
{
auto border=this->GetBorderBrush();
if(border)
	rc.SetPadding(1, 1, 1, 1);
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
RECT rc_move(rc);
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
	switch(AlignChildren)
		{
		case Alignment::Alternate:
			{
			if(uOrientation==Orientation::Horizontal)
				{
				rc_move.Top=rc.Bottom-child_size.Height;
				}
			else
				{
				rc_move.Left=rc.Right-child_size.Width;
				}
			break;
			}
		case Alignment::Center:
			{
			if(uOrientation==Orientation::Horizontal)
				{
				rc_move.Top=(rc.Bottom-child_size.Height)/2;
				}
			else
				{
				rc_move.Left=(rc.Right-child_size.Width)/2;
				}
			break;
			}
		case Alignment::Stretch:
			{
			if(uOrientation==Orientation::Horizontal)
				{
				child_size.Height=rc.Bottom-rc.Top;
				}
			else
				{
				child_size.Width=rc.Right-rc.Left;
				}
			break;
			}
		default:
			{
			break;
			}
		}
	rc_move.Right=rc_move.Left+child_size.Width;
	rc_move.Bottom=rc_move.Top+child_size.Height;
	RECT rc_child(rc_move);
	if(control)
		rc_child.SetPadding(control->Margin*scale);
	child->Move(target, rc_child);
	if(uOrientation==Orientation::Horizontal)
		{
		rc_move.Left=rc_move.Right;
		}
	else
		{
		rc_move.Top=rc_move.Bottom;
		}
	}
}

}}