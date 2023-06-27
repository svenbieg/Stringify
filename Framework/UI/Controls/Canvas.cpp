//============
// Canvas.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Canvas.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Canvas::Canvas(Window* parent):
Control(parent)
{}


//========
// Common
//========

Graphics::SIZE Canvas::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	RECT const& rc=child->GetRect();
	UINT right=rc.Right;
	UINT bottom=rc.Bottom;
	auto control=Convert<Control>(child);
	if(control)
		{
		RECT const& margin=control->Margin;
		right+=margin.Right;
		bottom+=margin.Bottom;
		}
	size.Width=MAX(size.Width, right);
	size.Height=MAX(size.Height, bottom);
	}
FLOAT scale=GetScaleFactor();
return size.Max(MinSize*scale);
}

}}