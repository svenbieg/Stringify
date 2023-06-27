//==========
// Grid.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Grid.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Grid::Grid(Window* parent):
Control(parent)
{
Children->Added.Add(this, &Grid::OnChildAdded);
Children->Removed.Add(this, &Grid::OnChildRemoved);
Columns=new ColumnList();
Rows=new RowList();
}


//========
// Common
//========

Handle<GridColumn> Grid::AddColumn(UINT width, GridUnit unit, HorizontalAlignment align)
{
auto column=new GridColumn(width, unit, align);
Columns->Append(column);
return column;
}

Handle<GridRow> Grid::AddRow(UINT height, GridUnit unit, VerticalAlignment align)
{
auto row=new GridRow(height, unit, align);
Rows->Append(row);
return row;
}

Graphics::SIZE Grid::GetMinSize(RenderTarget* target)
{
InitRaster(target);
SIZE size(GridMinSize(cColumns), GridMinSize(cRows));
FLOAT scale=GetScaleFactor();
size.AddPadding(Padding*scale);
return size.Max(MinSize*scale);
}

VOID Grid::Rearrange(RenderTarget* target, RECT& rc)
{
InitRaster(target);
FLOAT scale=GetScaleFactor();
rc.SetPadding(Padding*scale);
UpdateRaster(rc);
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child)
		continue;
	if(!child->Visible)
		continue;
	SIZE min_size=child->GetMinSize(target);
	RECT margin(0, 0, 0, 0);
	auto control=Convert<Control>(child);
	if(control)
		margin=control->Margin*scale;
	RECT rc_child=GetChildRect(child, min_size, margin);
	rc_child.Move(rc.Left, rc.Top);
	child->Move(target, rc_child);
	}
}

VOID Grid::SetPosition(UINT col, UINT row)
{
cPosition.Column=col;
cPosition.Row=row;
}


//================
// Common Private
//================

Graphics::RECT Grid::GetChildRect(Window* child, SIZE const& min_size, RECT const& margin)
{
GridPosition pos;
cPositions.try_get(child, &pos);
HorizontalAlignment align_h=HorizontalAlignment::Stretch;
auto col=Columns->GetAt(pos.Column);
if(col)
	align_h=col->Alignment;
VerticalAlignment align_v=VerticalAlignment::Stretch;
auto row=Rows->GetAt(pos.Row);
if(row)
	align_v=row->Alignment;
GridSize const& col_size=cColumns.get_at(pos.Column);
GridSize const& row_size=cRows.get_at(pos.Row);
RECT rc;
rc.Left=col_size.Offset+margin.Left;
rc.Right=col_size.Offset+col_size.SetSize-margin.Right;
rc.Top=row_size.Offset+margin.Top;
rc.Bottom=row_size.Offset+row_size.SetSize-margin.Bottom;
switch(align_h)
	{
	case HorizontalAlignment::Left:
		{
		rc.Right=rc.Left+min_size.Width;
		break;
		}
	case HorizontalAlignment::Right:
		{
		rc.Left=rc.Right-min_size.Width;
		break;
		}
	case HorizontalAlignment::Center:
		{
		rc.Left=col_size.Offset+(col_size.SetSize-min_size.Width)/2;
		break;
		}
	default:
		{
		break;
		}
	}
switch(align_v)
	{
	case VerticalAlignment::Top:
		{
		rc.Bottom=rc.Top+min_size.Height;
		break;
		}
	case VerticalAlignment::Bottom:
		{
		rc.Top=rc.Bottom-min_size.Height;
		break;
		}
	case VerticalAlignment::Center:
		{
		rc.Top=row_size.Offset+(row_size.SetSize-min_size.Height)/2;
		break;
		}
	default:
		{
		break;
		}
	}
return rc;
}

VOID Grid::InitColumns(UINT count, FLOAT scale)
{
InitSizes(cColumns, count);
for(UINT u=0; u<count; u++)
	{
	auto col=Columns->GetAt(u);
	GridSize& size=cColumns[u];
	size.MinSize=0;
	size.Offset=0;
	size.SetSize=0;
	size.Size=col? col->Width: 1;
	size.Unit=col? col->Unit: GridUnit::Star;
	if(size.Unit==GridUnit::Pixel)
		size.MinSize=size.Size*scale;
	}
}

VOID Grid::InitRaster(RenderTarget* target)
{
UINT col_count=cColumns.get_count();
UINT row_count=cRows.get_count();
for(auto it=cPositions.cbegin(); it.has_current(); it.move_next())
	{
	auto obj=it->get_key();
	auto child=Convert<Control>(obj);
	if(!child)
		continue;
	auto pos=it->get_value();
	col_count=MAX(col_count, pos.Column+1);
	row_count=MAX(row_count, pos.Row+1);
	}
col_count=MAX(col_count, 1);
row_count=MAX(row_count, 1);
FLOAT scale=GetScaleFactor();
InitColumns(col_count, scale);
InitRows(row_count, scale);
for(auto it=Children->First(); it->HasCurrent(); it->MoveNext())
	{
	auto child=it->GetCurrent();
	if(!child)
		continue;
	if(!child->Visible)
		continue;
	GridPosition pos(0, 0);
	cPositions.try_get(child, &pos);
	SIZE min_size=child->GetMinSize(target);
	auto control=Convert<Control>(child);
	if(control)
		{
		RECT const& margin=control->Margin;
		min_size.AddPadding(margin*scale);
		}
	GridSize& col=cColumns.get_at(pos.Column);
	GridSize& row=cRows.get_at(pos.Row);
	col.MinSize=MAX(col.MinSize, min_size.Width);
	row.MinSize=MAX(row.MinSize, min_size.Height);
	}
}

VOID Grid::InitRows(UINT count, FLOAT scale)
{
InitSizes(cRows, count);
for(UINT u=0; u<count; u++)
	{
	auto row=Rows->GetAt(u);
	GridSize& size=cRows[u];
	size.MinSize=0;
	size.Offset=0;
	size.SetSize=0;
	size.Size=row? row->Height: 1;
	size.Unit=row? row->Unit: GridUnit::Star;
	if(size.Unit==GridUnit::Pixel)
		size.MinSize=size.Size*scale;
	}
}

VOID Grid::InitSizes(GridSizeList& sizes, UINT count)
{
while(sizes.get_count()<count)
	sizes.append(GridSize(1, GridUnit::Star));
while(sizes.get_count()>count)
	sizes.remove_at(count);
}

UINT Grid::GridMinSize(GridSizeList const& sizes)
{
UINT min_size=0;
for(auto it=sizes.cbegin(); it.has_current(); it.move_next())
	{
	GridSize const& size=it.get_current();
	if(size.Unit==GridUnit::Star)
		continue;
	min_size+=size.MinSize;
	}
return min_size;
}

VOID Grid::OnChildAdded(Handle<Window> child)
{
UINT col_count=Columns->GetCount();
if(col_count==0)
	col_count=1;
cPositions[child]=cPosition;
cPosition.Column++;
if(cPosition.Column==col_count)
	{
	cPosition.Column=0;
	cPosition.Row++;
	}
}

VOID Grid::OnChildRemoved(Handle<Window> child)
{
cPositions.remove(child);
Invalidate(true);
}

VOID Grid::SetOffsets(GridSizeList& sizes, UINT start)
{
UINT offset=start;
for(auto it=sizes.begin(); it.has_current(); it.move_next())
	{
	GridSize& size=it.get_current();
	size.Offset=offset;
	offset+=size.SetSize;
	}
}

VOID Grid::SetPixels(GridSizeList& sizes, UINT total)
{
for(auto it=sizes.begin(); it.has_current(); it.move_next())
	{
	GridSize& size=it.get_current();
	switch(size.Unit)
		{
		case GridUnit::Auto:
			{
			size.SetSize=size.MinSize;
			break;
			}
		case GridUnit::Percent:
			{
			UINT set=size.Size*total/100;
			size.SetSize=set;
			break;
			}
		case GridUnit::Pixel:
			{
			FLOAT scale=GetScaleFactor();
			size.SetSize=size.Size*scale;
			break;
			}
		default:
			{
			break;
			}
		}
	}
}

VOID Grid::SetStars(GridSizeList& sizes, UINT total)
{
UINT count=sizes.get_count();
UINT stars=0;
UINT sum=0;
for(UINT u=0; u<count; u++)
	{
	GridSize const& size=sizes.get_at(u);
	if(size.Unit==GridUnit::Star)
		{
		stars+=size.Size;
		continue;
		}
	sum+=size.SetSize;
	}
if(sum>=total)
	return;
UINT space=total-sum;
if(stars==0)
	{
	sizes[count-1].SetSize+=space;
	return;
	}
UINT star=space/stars;
for(UINT u=0; u<count; u++)
	{
	GridSize& size=sizes.get_at(u);
	if(size.Unit!=GridUnit::Star)
		continue;
	UINT set=MAX(size.MinSize, size.Size*star);
	if(set>space)
		{
		size.SetSize=space;
		space=0;
		star=0;
		}
	else
		{
		size.SetSize=set;
		space-=set;
		}
	if(--stars==0)
		break;
	}
}

VOID Grid::UpdateRaster(RECT const& rc)
{
SIZE size=rc.GetSize();
SetPixels(cColumns, size.Width);
SetPixels(cRows, size.Height);
SetStars(cColumns, size.Width);
SetStars(cRows, size.Height);
SetOffsets(cColumns, Padding.Left);
SetOffsets(cRows, Padding.Top);
}

}}