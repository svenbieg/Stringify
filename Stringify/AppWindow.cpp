//===============
// AppWindow.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Graphics/Direct2D/Font.h"
#include "Resources/Icons/Icons.rc"
#include "UI/Controls/Group.h"
#include "UI/Controls/PathEdit.h"
#include "Application.h"

using namespace Graphics::Direct2D;
using namespace Resources::Icons;
using namespace UI;
using namespace UI::Controls;


//===========
// Namespace
//===========

namespace Stringify {


//==================
// Con-/Destructors
//==================

AppWindow::AppWindow():
Desktop::AppWindow(ICO_APP, AppWindowStyle::DialogWindow)
{
MinSize.Set(480, 512);
Body->Padding.Set(8, 8, 8, 8);
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
ResultBox->Font=new Font("Lucida Sans Typewriter");
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