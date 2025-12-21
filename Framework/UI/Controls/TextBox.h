//===========
// TextBox.h
//===========

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Input.h"
#include "UI/Controls/ScrollBox.h"


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
	using IInputStream=Storage::Streams::IInputStream;
	using StringList=Collections::StringList;

	// Con-/Destructors
	static inline Handle<TextBox> Create(Window* Parent) { return new TextBox(Parent); }

	// Common
	VOID AppendLine(Handle<String> Line);
	VOID AppendLines(Handle<StringList> Lines);
	VOID Clear();
	Handle<Brush> GetBackground()override;
	VOID ReadFromStream(IInputStream* Stream);
	Property<TextBox, BOOL> ReadOnly;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	VOID SelectAll();
	Event<TextBox> SelectionChanged;
	VOID SetFocus();
	VOID SetText(Handle<String> Text);

private:
	// Con-/Destructors
	TextBox(Window* Parent);

	// Common
	VOID OnEnabledChanged(BOOL Enabled);
	VOID OnInputSelectionChanged();
	VOID OnReadOnlyChanged(BOOL ReadOnly);
	Handle<Input> m_Input;
};

}}