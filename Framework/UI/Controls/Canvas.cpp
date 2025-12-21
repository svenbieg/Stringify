//============
// Canvas.cpp
//============

#include "Canvas.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Graphics::SIZE Canvas::GetMinSize(RenderTarget* target)
{
SIZE size(0, 0);
for(auto it=Children->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child->Visible)
		continue;
	RECT const& rc=child->GetRect();
	UINT right=rc.Right;
	UINT bottom=rc.Bottom;
	auto control=child.As<Control>();
	if(control)
		{
		RECT const& margin=control->Margin;
		right+=margin.Right;
		bottom+=margin.Bottom;
		}
	size.Width=TypeHelper::Max(size.Width, right);
	size.Height=TypeHelper::Max(size.Height, bottom);
	}
FLOAT scale=GetScaleFactor();
return size.Max(MinSize*scale);
}

}}