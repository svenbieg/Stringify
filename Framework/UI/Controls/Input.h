//=========
// Input.h
//=========

#pragma once


//=======
// Using
//=======

#include "Collections/list.hpp"
#include "Collections/map.hpp"
#include "Collections/StringList.h"
#include "Storage/Streams/InputStream.h"
#include "Timing/Timer.h"
#include "UI/Controls/Menus/PopupMenu.h"
#include "UI/Controls/Interactive.h"
#include "StringBuilder.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//=======
// Input
//=======

class Input: public Interactive
{
public:
	// Using
	using InputStream=Storage::Streams::InputStream;
	using PopupMenu=UI::Controls::Menus::PopupMenu;
	using StringList=Collections::StringList;
	using Timer=Timing::Timer;

	// Con-/Destructors
	static inline Handle<Input> Create(Window* Parent) { return new Input(Parent); }

	// Common
	VOID AppendLine(Handle<String> Line);
	VOID AppendLines(Handle<StringList> Lines);
	VOID Clear();
	VOID ClearSelection();
	Handle<PopupMenu> ContextMenu;
	Handle<Cursor> GetCursor()override;
	RECT GetCursorRect();
	UINT GetLineHeight()const;
	SIZE GetMinSize(RenderTarget* Target)override;
	Handle<String> GetSelection();
	Handle<String> GetText();
	BOOL KillFocus()override;
	TCHAR Mask;
	BOOL MultiLine;
	RECT Padding;
	VOID ReadFromStream(InputStream* Stream);
	BOOL ReadOnly;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	VOID ReplaceSelection(LPCTSTR Value);
	VOID SelectAll();
	VOID SelectNone();
	Event<Input> SelectionChanged;
	VOID SetFocus(FocusReason Reason=FocusReason::None)override;
	VOID SetSelection(POINT const& Start, POINT const& End);
	VOID SetText(Handle<String> Text);

private:
	// Line
	typedef struct
		{
		Collections::list<UINT> Offsets;
		Handle<String> Text;
		}INPUT_LINE;

	// Flags
	enum class InputFlags: UINT
		{
		None=0,
		Pointing=1,
		Shift=2
		};

	// Con-/Destructors
	Input(Window* Parent);

	// Common
	POINT CharFromPoint(POINT Point, UINT LineHeight);
	POINT GetEndPoint();
	UINT GetLineHeight(RenderTarget* Target, FLOAT Scale);
	UINT GetLineWidth(INPUT_LINE const& Line, FLOAT ScaleFactor);
	UINT GetText(POINT const& Start, POINT const& End, StringBuilder& Builder);
	UINT GetText(POINT const& Start, POINT const& End, LPTSTR Buffer, UINT Size);
	UINT GetTextLength(POINT const& Start, POINT const& End);
	VOID MoveCursor(INT X, INT Y, BOOL Select, BOOL Notify=true);
	VOID NotifySelectionChanged();
	VOID OnCursorTimer();
	VOID OnFocused();
	VOID OnFocusLost();
	VOID OnKey(Handle<KeyEventArgs> Args);
	VOID OnKeyDown(Handle<KeyEventArgs> Args);
	VOID OnKeyUp(Handle<KeyEventArgs> Args);
	VOID OnPointerDown(Handle<PointerEventArgs> Args);
	VOID OnPointerMoved(Handle<PointerEventArgs> Args);
	VOID OnPointerUp(Handle<PointerEventArgs> Args);
	POINT PointFromChar(POINT const& Char, FLOAT ScaleFactor);
	BOOL PointIsSelected(POINT const& Point);
	BOOL ShowContextMenu(POINT Point);
	VOID ShowCursor(BOOL Show);
	VOID UpdateLine(INPUT_LINE& Line);
	VOID UpdatePointer();
	VOID UpdateSelection();
	POINT m_CursorPos;
	UINT m_CursorTime;
	Handle<Timer> m_CursorTimer;
	InputFlags m_InputFlags;
	Collections::list<INPUT_LINE> m_Lines;
	UINT m_LineHeight;
	Collections::map<UINT, UINT> m_LineWidths;
	POINT m_PointerPos;
	POINT m_SelectionEnd;
	POINT m_SelectionFirst;
	POINT m_SelectionLast;
	POINT m_SelectionStart;
};

}}