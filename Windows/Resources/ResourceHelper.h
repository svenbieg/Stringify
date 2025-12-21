//==================
// ResourceHelper.h
//==================

#pragma once


//=======
// Using
//=======

#include "Platform.h"


//===========
// Namespace
//===========

namespace Resources {


//=================
// Resource-Helper
//=================

class ResourceHelper
{
public:
	// Common
	static BITMAPINFO* GetIcon(UINT Id, UINT Size);
	static UINT GetIconCount(UINT Id);
	static UINT GetIconSize(UINT Id, UINT* Size, UINT Count);
};

}