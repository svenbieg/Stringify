//===============
// AppWindow.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Graphics/Direct2D/Font.h"
#include "Graphics/Direct2D/Icon.h"
#include "UI/Controls/Group.h"
#include "UI/Controls/PathEdit.h"
#include "Application.h"

using namespace Graphics::Direct2D;
using namespace Resources::Icons;
using namespace UI;
using namespace UI::Controls;

using D2DFont=Graphics::Direct2D::Font;
using D2DIcon=Graphics::Direct2D::Icon;


//===========
// Namespace
//===========

namespace Stringify {


//==================
// Con-/Destructors
//==================

AppWindow::AppWindow()
{
Body->Padding.Set(8, 8, 8, 8);
Icon=new D2DIcon(ICO_APP);
MinSize.Set(480, 512);
auto grid=new Grid(Body);
grid->AddColumn(1, GridUnit::Star);
grid->AddRow(1, GridUnit::Auto);
grid->AddRow(1, GridUnit::Star);
auto group=new Group(grid, "Binary");
auto edit=new PathEdit(group);
edit->MinSize.Set(400, 0);
edit->Path.Changed.Add(this, &AppWindow::OnPathEditPathChanged);
group=new Group(grid, "String");
group->Margin.Set(0, 8, 0, 0);
ResultBox=new TextBox(group);
ResultBox->Enabled=false;
ResultBox->Font=new D2DFont("Lucida Console");
Shrink();
}


//================
// Common Private
//================

VOID AppWindow::OnPathEditPathChanged(Handle<String> path)
{
auto app=Application::Current;
app->Open(path);
}

}
