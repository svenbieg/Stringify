//===========
// TextBox.h
//===========

#pragma once


//=======
// Using
//=======

#include "Input.h"
#include "ScrollBox.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==========
// Text-Box
//==========

class TextBox: public ScrollBox
{
public:
	// Using
	using InputStream=Storage::Streams::InputStream;
	using StringList=Collections::StringList;

	// Con-/Destructors
	TextBox(Window* Parent);

	// Common
	VOID AppendLine(Handle<String> Line);
	VOID AppendLines(Handle<StringList> Lines);
	VOID Clear();
	VOID ReadFromStream(Handle<InputStream> Stream);
	Property<TextBox, BOOL> ReadOnly;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	VOID SelectAll();
	Event<TextBox> SelectionChanged;
	VOID SetFocus();
	VOID SetText(Handle<String> Text);

private:
	// Common
	VOID OnInputSelectionChanged();
	VOID OnReadOnlyChanged(BOOL ReadOnly);
	Handle<Input> m_Input;
};

}}