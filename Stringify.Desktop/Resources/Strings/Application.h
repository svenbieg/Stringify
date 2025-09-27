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

constexpr LPCSTR STR_APP_TITLE="Stringify";

constexpr STRING STR_SETTINGS[]=
	{
	{ LNG::DE, L"Einstellungen" },
	{ LNG::EN, L"Settings" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_SOURCE[]=
	{
	{ LNG::DE, L"Quelle" },
	{ LNG::EN, L"Source" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_STRING[]=
	{
	{ LNG::DE, L"Zeichenfolge" },
	{ LNG::EN, L"String" },
	{ LNG::None, nullptr }
	};

}}