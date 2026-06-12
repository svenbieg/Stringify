//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Resources/Strings/String.h"


//===========
// Namespace
//===========

namespace Resources {
	namespace Strings {


//=============
// Application
//=============

const LPCSTR STR_APP_TITLE="Stringify";

const STRING STR_SOURCE[]=
	{
	{ LNG::DE, L"Quelle" },
	{ LNG::EN, L"Source" },
	{ LNG::None, nullptr }
	};

const STRING STR_STRING[]=
	{
	{ LNG::DE, L"Zeichenfolge" },
	{ LNG::EN, L"String" },
	{ LNG::None, nullptr }
	};

}}