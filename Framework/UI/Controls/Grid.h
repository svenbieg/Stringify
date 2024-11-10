//========
// Grid.h
//========

#pragma once


//=======
// Using
//=======

#include "Collections/list.hpp"
#include "Collections/map.hpp"
#include "UI/Alignment.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//======
// Unit
//======

enum class GridUnit: UINT
{
Auto, Percent, Pixel, Star
};


//========
// Column
//========

class GridColumn: public Object
{
public:
	// Con-/Destructors
	GridColumn(UINT Width, GridUnit Unit, HorizontalAlignment Alignment):
		Alignment(Alignment), Unit(Unit), Width(Width) {}

	// Common
	HorizontalAlignment Alignment;
	GridUnit Unit;
	UINT Width;
};


//=====
// Row
//=====

class GridRow: public Object
{
public:
	// Con-/Destructors
	GridRow(UINT Height, GridUnit Unit, VerticalAlignment Alignment):
		Alignment(Alignment), Height(Height), Unit(Unit) {}

	// Common
	VerticalAlignment Alignment;
	GridUnit Unit;
	UINT Height;
};


//======
// Grid
//======

class Grid: public Control
{
public:
	// Using
	using ColumnList=Collections::List<Handle<GridColumn>>;
	using RowList=Collections::List<Handle<GridRow>>;

	// Con-/Destructors
	Grid(Window* Parent=nullptr);

	// Common
	Handle<GridColumn> AddColumn(UINT Width=0, GridUnit Unit=GridUnit::Pixel, HorizontalAlignment Alignment=HorizontalAlignment::Stretch);
	Handle<GridRow> AddRow(UINT Height=0, GridUnit Unit=GridUnit::Pixel, VerticalAlignment Alignment=VerticalAlignment::Stretch);
	Grid* At(UINT Column, UINT Row);
	Handle<ColumnList> Columns;
	SIZE GetMinSize(RenderTarget* Target)override;
	RECT Padding;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	Handle<RowList> Rows;

private:
	// Grid-Position
	class GridPosition
		{
		public:
			GridPosition(UINT Column=0, UINT Row=0):
				Column(Column),
				Row(Row)
				{}
			UINT Column;
			UINT Row;
		};
	using GridPositionMap=Collections::map<Window*, GridPosition>;

	// Grid-Size
	class GridSize
		{
		public:
			GridSize(UINT Size=0, GridUnit Unit=GridUnit::Pixel):
				MinSize(0), Offset(0), SetSize(0), Size(Size), Unit(Unit) {}
			UINT MinSize;
			UINT Offset;
			UINT SetSize;
			UINT Size;
			GridUnit Unit;
		};
	using GridSizeList=Collections::list<GridSize>;

	// Common
	Graphics::RECT GetChildRect(Window* Child, Graphics::SIZE const& MinSize, Graphics::RECT const& Margin);
	UINT GridMinSize(GridSizeList const& Sizes);
	VOID InitColumns(UINT Count, FLOAT Scale);
	VOID InitRaster(RenderTarget* Target);
	VOID InitRows(UINT Count, FLOAT Scale);
	VOID InitSizes(GridSizeList& Sizes, UINT Count);
	VOID OnChildAdded(Handle<Window> Child);
	VOID OnChildRemoved(Handle<Window> Child);
	VOID SetOffsets(GridSizeList& Sizes, UINT Start);
	VOID SetPixels(GridSizeList& Sizes, UINT Total);
	VOID SetStars(GridSizeList& Sizes, UINT Total);
	VOID UpdateRaster(Graphics::RECT const& Rect);
	GridSizeList m_Columns;
	GridPosition m_Position;
	GridPositionMap m_Positions;
	GridSizeList m_Rows;
};

}}
