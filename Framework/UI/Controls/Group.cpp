//===========
// Group.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Group.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Group::Group(Window* parent, Handle<String> title):
Control(parent),
Padding(8, 8, 8, 8),
Title(title)
{}


//========
// Common
//========

Graphics::SIZE Group::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
SIZE title_size(0, 0);
if(Title)
	{
	auto font=GetFont();
	title_size=target->MeasureText(font, scale, Title->Begin());
	}
SIZE content_size(0, 0);
auto content=GetVisibleChild(0);
if(content)
	{
	content_size=content->GetMinSize(target);
	auto control=Convert<Control>(content);
	if(control)
		{
		RECT const& margin=control->Margin;
		content_size.AddPadding(margin*scale);
		}
	}
SIZE size;
size.Width=MAX(title_size.Width, content_size.Width);
size.Height=title_size.Height+content_size.Height;
size.AddPadding(Padding*scale);
size.AddPadding(1, 1, 1, 1);
return size.Max(MinSize*scale);
}

VOID Group::Rearrange(RenderTarget* target, RECT& rc)
{
auto content=GetVisibleChild(0);
if(!content)
	return;
rc.SetPadding(1, 1, 1, 1);
FLOAT scale=GetScaleFactor();
SIZE title_size(0, 0);
if(Title)
	{
	auto theme=GetTheme();
	auto font=theme->GetDefaultFont();
	title_size=target->MeasureText(font, scale, Title->Begin());
	}
rc.Top+=title_size.Height;
rc.SetPadding(Padding*scale);
auto control=Convert<Control>(content);
if(control)
	{
	RECT const& margin=control->Margin;
	rc.SetPadding(margin*scale);
	}
content->Move(target, rc);
}

VOID Group::Render(RenderTarget* target, RECT& rc)
{
Control::Render(target, rc);
auto theme=GetTheme();
auto border=theme->GetBorderBrush();
RECT rc_client=GetClientRect();
if(Title)
	{
	auto font=GetFont();
	FLOAT scale=GetScaleFactor();
	SIZE title_size=target->MeasureText(font, scale, Title->Begin());
	RECT rc_title;
	rc_title.Left=rc_client.Right/2-title_size.Width/2;
	rc_title.Top=0;
	rc_title.Right=rc_title.Left+title_size.Width;
	rc_title.Bottom=title_size.Height;
	INT top=title_size.Height/2;
	UINT space=2*scale;
	target->DrawLine(POINT(rc_client.Left, top), POINT(rc_title.Left-space, top), border);
	target->DrawLine(POINT(rc_title.Right+space, top), POINT(rc_client.Right, top), border);
	target->DrawLine(POINT(rc_client.Left, top), POINT(rc_client.Left, rc_client.Bottom), border);
	target->DrawLine(POINT(rc_client.Right, top), POINT(rc_client.Right, rc_client.Bottom), border);
	target->DrawLine(POINT(rc_client.Left, rc_client.Bottom), POINT(rc_client.Right, rc_client.Bottom), border);
	target->TextColor=theme->GetTextBrush();
	target->Font=font;
	target->DrawText(rc_title, scale, Title->Begin());
	rc.SetPadding(1, title_size.Height, 1, 1);
	}
else
	{
	target->DrawRect(rc_client, border);
	rc.SetPadding(1, 1, 1, 1);
	}
}

}}