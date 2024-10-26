//==========
// Size.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Rect.h"
#include "Size.h"


//===========
// Namespace
//===========

namespace Graphics {


//==============
// Modification
//==============

VOID SIZE::AddPadding(RECT const& rc)
{
Width+=rc.Left+rc.Right;
Height+=rc.Top+rc.Bottom;
}

VOID SIZE::AddPadding(INT left, INT top, INT right, INT bottom)
{
Width+=left+right;
Height+=top+bottom;
}

}