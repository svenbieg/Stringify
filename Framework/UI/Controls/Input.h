//=========
// Input.h
//=========

#pragma once


//=======
// Using
//=======

#include "Clusters/list.hpp"
#include "Clusters/map.hpp"
#include "Core/Timer.h"
#include "Storage/Streams/InputStream.h"
#include "UI/Controls/Menus/PopupMenu.h"
#include "Interactive.h"


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

	// Con-/Destructors
	Input(Window* Parent);

	// Common
	VOID AppendLine(Handle<String> Text);
	VOID Clear();
	VOID ClearSelection();
	Handle<PopupMenu> ContextMenu;
	Handle<Brush> GetBackgroundBrush()override;
	Handle<Cursor> GetCursor()override;
	RECT GetCursorRect();
	UINT GetLineHeight()const;
	SIZE GetMinSize(RenderTarget* Target)override;
	Handle<String> GetSelection();
	Handle<String> GetText();
	TCHAR Mask;
	BOOL MultiLine;
	RECT Padding;
	VOID ReadFromStream(InputStream* Stream);
	BOOL ReadOnly;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	VOID ReplaceSelection(LPCTSTR Value);
	VOID SelectAll();
	Event<Input> SelectionChanged;
	VOID SetSelection(POINT const& Start, POINT const& End);
	VOID SetText(Handle<String> Text);

private:
	// Using
	using Timer=Core::Timer;

	// Line
	typedef struct
		{
		Clusters::list<UINT> Offsets;
		Handle<String> Text;
		}INPUT_LINE;

	// Flags
	enum class InputFlags: UINT
		{
		None=0,
		Pointing=1,
		Shift=2
		};

	// Common
	POINT CharFromPoint(POINT Point, UINT LineHeight);
	POINT GetEndPoint();
	UINT GetLineHeight(RenderTarget* Target, FLOAT Scale);
	UINT GetLineWidth(INPUT_LINE const& Line, FLOAT ScaleFactor);
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
	Clusters::list<INPUT_LINE> cLines;
	Clusters::map<UINT, UINT> cLineWidths;
	Handle<Timer> hCursorTimer;
	POINT ptCursor;
	POINT ptPointer;
	POINT ptSelectionEnd;
	POINT ptSelectionFirst;
	POINT ptSelectionLast;
	POINT ptSelectionStart;
	UINT uCursorTime;
	volatile InputFlags uInputFlags;
	UINT uLineHeight;
};

}}