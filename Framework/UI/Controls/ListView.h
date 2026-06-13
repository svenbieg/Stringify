//============
// ListView.h
//============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Interactive.h"
#include "Storage/Xml/Xml.h"
#include "DynamicHandle.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//===========
// List-View
//===========

class ListView: public Interactive
{
public:
	// Using
	using Xml=Storage::Xml::Xml;

	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<ListView> Create(Window* Parent)
		{
		return Object::Create<ListView>(Parent);
		}

	// Common
	Property<ListView, UINT> Columns;
	Handle<Brush> GetBackground()override;
	SIZE GetMinSize(RenderTarget* Target)override;
	Property<ListView, SIZE> ItemSize;
	RECT Padding;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	DynamicHandle<ListView, Xml> Source;

protected:
	// Con-/Destructors
	ListView(Window* Parent);

private:
	// Common
	POINT GetItemPosition(UINT Item, FLOAT Scale);
	UINT GetLineHeight(RenderTarget* Target, FLOAT Scale);
	VOID OnColumnsChanged();
	VOID OnItemSizeChanged();
	VOID OnSourceChanged();
	SIZE m_ItemSize;
	UINT m_SelectionEnd;
	UINT m_SelectionFirst;
	UINT m_SelectionLast;
	UINT m_SelectionStart;
};

}}