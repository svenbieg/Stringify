//===================
// ExceptionHelper.h
//===================

#pragma once


//=======
// Using
//=======

#include "TypeHelper.h"


//========
// Common
//========

class ExceptionHelper
{
public:
	// Common
	static UINT PrintContext(CONTEXT* Context, UINT Levels, LPSTR String, UINT Size);

private:
	// Common
	static BOOL LoadSymbols();
};
