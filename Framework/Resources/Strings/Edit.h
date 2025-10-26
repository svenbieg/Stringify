//=========
// Input.h
//=========

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


//========
// Common
//========

constexpr STRING STR_MM_EDIT_COPY[]=
	{
	{ LNG::DE, L"&Kopieren|Strg+C" },
	{ LNG::EN, L"&Copy|Ctrl+C" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MM_EDIT_CUT[]=
	{
	{ LNG::DE, L"&Ausschneiden|Strg+X" },
	{ LNG::EN, L"&Cut|Ctrl+X" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MM_EDIT_DELETE[]=
	{
	{ LNG::DE, L"&Löschen|Entf" },
	{ LNG::EN, L"&Delete|Del" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MM_EDIT_PASTE[]=
	{
	{ LNG::DE, L"&Einfügen|Strg+V" },
	{ LNG::EN, L"&Paste|Ctrl+V" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MM_EDIT_SELECT_ALL[]=
	{
	{ LNG::DE, L"&Alles Auswählen|Strg+A" },
	{ LNG::EN, L"Select &all|Ctrl+A" },
	{ LNG::None, nullptr }
	};

}}