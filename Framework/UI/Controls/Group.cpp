//===========
// Group.cpp
//===========

#include "UI/Controls/Group.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Graphics::SIZE Group::GetMinSize(RenderTarget* target)
{
FLOAT scale=GetScaleFactor();
SIZE title_size(0, 0);
if(Title)
	{
	auto font=m_Theme->DefaultFont;
	title_size=target->MeasureText(font, scale, Title->Begin());
	}
SIZE content_size(0, 0);
auto content=GetVisibleChild(0);
if(content)
	{
	content_size=content->GetMinSize(target);
	auto control=content.As<Control>();
	if(control)
		{
		RECT const& margin=control->Margin;
		content_size.AddPadding(margin*scale);
		}
	}
SIZE size;
size.Width=TypeHelper::Max(title_size.Width, content_size.Width);
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
	auto font=m_Theme->DefaultFont;
	title_size=target->MeasureText(font, scale, Title->Begin());
	}
rc.Top+=title_size.Height;
rc.SetPadding(Padding*scale);
auto control=content.As<Control>();
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
auto border=m_Theme->BorderBrush;
RECT rc_client=GetClientRect();
if(Title)
	{
	auto font=m_Theme->DefaultFont;
	auto brush=m_Theme->TextBrush;
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
	target->DrawText(rc_title, scale, font, brush, Title->Begin());
	rc.SetPadding(1, title_size.Height, 1, 1);
	}
else
	{
	target->DrawRect(rc_client, border);
	rc.SetPadding(1, 1, 1, 1);
	}
}


//==========================
// Con-/Destructors Private
//==========================

Group::Group(Window* parent, Handle<Sentence> title):
Control(parent),
Padding(8, 8, 8, 8),
Title(this, title)
{
Title.Changed.Add(this, &Group::OnTitleChanged);
}


//================
// Common Private
//================

VOID Group::OnTitleChanged()
{
Invalidate(true);
}

}}