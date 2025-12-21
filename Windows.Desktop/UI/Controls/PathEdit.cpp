//==============
// PathEdit.cpp
//==============

#include "UI/Controls/PathEdit.h"


//=======
// Using
//=======

#include "Desktop/Dialogs/OpenFileDialog.h"
#include "Desktop/Dialogs/SaveFileDialog.h"

using namespace Desktop::Dialogs;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==========================
// Con-/Destructors Private
//==========================

PathEdit::PathEdit(Window* parent, PathEditMode mode):
Grid(parent),
Mode(mode),
Path(this)
{
Path.Changed.Add(this, &PathEdit::OnPathChanged);
AddColumn(1, GridUnit::Star);
AddColumn(1, GridUnit::Auto);
AddRow(1, GridUnit::Auto, VerticalAlignment::Center);
m_EditBox=EditBox::Create(this);
m_Button=Button::Create(this, "..");
m_Button->Border=true;
m_Button->Clicked.Add(this, &PathEdit::OnButtonClicked);
m_Button->Margin.Set(-1, 0, 0, 0);
m_Button->Padding.Set(4, 2, 4, 2);
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
m_EditBox->Text=path;
}

}}