//===============
// AppWindow.cpp
//===============

#include "AppWindow.h"


//=======
// Using
//=======

#include "Resources/Strings/Application.h"
#include "UI/Controls/Group.h"
#include "UI/Controls/PathEdit.h"
#include "Application.h"

using namespace Graphics::Direct2D;
using namespace Resources::Icons;
using namespace Resources::Strings;
using namespace UI;
using namespace UI::Controls;


//===========
// Namespace
//===========

namespace Stringify {


//==========================
// Con-/Destructors Private
//==========================

AppWindow::AppWindow()
{
Body->Padding.Set(8, 8, 8, 8);
MinSize.Set(480, 512);
auto grid=Grid::Create(Body);
grid->AddColumn(1, GridUnit::Star);
grid->AddRow(1, GridUnit::Auto);
grid->AddRow(1, GridUnit::Auto);
grid->AddRow(1, GridUnit::Star);
auto group=Group::Create(grid, STR_SOURCE);
auto edit=PathEdit::Create(group);
edit->MinSize.Set(400, 0);
edit->Path.Changed.Add(this, &AppWindow::OnPathEditPathChanged);
group=Group::Create(grid, STR_STRING);
group->Margin.Set(0, 8, 0, 0);
ResultBox=TextBox::Create(group);
ResultBox->Enabled=false;
ResultBox->ReadOnly=true;
Minimize(Minimization::Size);
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