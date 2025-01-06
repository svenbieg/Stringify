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

class PathEdit: public UI::Controls::Grid
{
public:
	// Using
	using Button=UI::Controls::Button;
	using EditBox=UI::Controls::EditBox;
	using Sentence=Culture::Sentence;

	// Con-/Destructors
	static inline Handle<PathEdit> Create(Window* Parent, PathEditMode Mode=PathEditMode::OpenFile)
		{
		return new PathEdit(Parent, Mode);
		}

	// Common
	Handle<Sentence> Filter;
	PathEditMode Mode;
	DynamicHandle<PathEdit, String> Path;

private:
	// Con-/Destructors
	PathEdit(Window* Parent, PathEditMode Mode);

	// Common
	VOID OnButtonClicked();
	VOID OnPathChanged(Handle<String> Path);
	Handle<Button> m_Button;
	Handle<EditBox> m_EditBox;
};

}}