//==============
// PathEdit.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Desktop/Dialogs/OpenFileDialog.h"
#include "Desktop/Dialogs/SaveFileDialog.h"
#include "PathEdit.h"

using namespace Desktop::Dialogs;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

PathEdit::PathEdit(Window* parent, PathEditMode mode):
Grid(parent),
Mode(mode),
Path(this)
{
Path.Changed.Add(this, &PathEdit::OnPathChanged);
AddColumn(1, GridUnit::Star);
AddColumn(1, GridUnit::Auto);
AddRow(1, GridUnit::Auto, VerticalAlignment::Center);
hEditBox=new EditBox(this);
hButton=new Button(this, "..");
hButton->Border=true;
hButton->Clicked.Add(this, &PathEdit::OnButtonClicked);
hButton->Margin.Set(-1, 0, 0, 0);
hButton->Padding.Set(4, 2, 4, 2);
}


//================
// Common Private
//================

VOID PathEdit::OnButtonClicked()
{
Handle<String> path;
switch(Mode)
	{
	case PathEditMode::OpenFile:
		{
		path=OpenFileDialog::Show(Path, Filter);
		break;
		}
	case PathEditMode::SaveFile:
		{
		path=SaveFileDialog::Show(Path, Filter);
		break;
		}
	}
if(path)
	{
	Path=path;
	}
}

VOID PathEdit::OnPathChanged(Handle<String> path)
{
hEditBox->Text=path;
}

}}