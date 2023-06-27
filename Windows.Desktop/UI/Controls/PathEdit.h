//============
// PathEdit.h
//============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Button.h"
#include "UI/Controls/EditBox.h"
#include "UI/Controls/Grid.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//======
// Mode
//======

enum class PathEditMode
{
OpenFile,
SaveFile
};


//===========
// Path-Edit
//===========

class PathEdit: public Grid
{
public:
	// Con-/Destructors
	PathEdit(Window* Parent, PathEditMode Mode=PathEditMode::OpenFile);

	// Common
	Handle<Sentence> Filter;
	PathEditMode Mode;
	DynamicHandle<PathEdit, String> Path;

private:
	// Common
	VOID OnButtonClicked();
	VOID OnPathChanged(Handle<String> Path);
	Handle<Button> hButton;
	Handle<EditBox> hEditBox;
};

}}