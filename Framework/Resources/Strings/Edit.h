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
	{ LNG::DE, "&Kopieren|Strg+C" },
	{ LNG::EN, "&Copy|Ctrl+C" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MM_EDIT_CUT[]=
	{
	{ LNG::DE, "&Ausschneiden|Strg+X" },
	{ LNG::EN, "&Cut|Ctrl+X" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MM_EDIT_DELETE[]=
	{
	{ LNG::DE, "&Löschen|Entf" },
	{ LNG::EN, "&Delete|Del" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MM_EDIT_PASTE[]=
	{
	{ LNG::DE, "&Einfügen|Strg+V" },
	{ LNG::EN, "&Paste|Ctrl+V" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_MM_EDIT_SELECT_ALL[]=
	{
	{ LNG::DE, "&Alles Auswählen|Strg+A" },
	{ LNG::EN, "Select &all|Ctrl+A" },
	{ LNG::None, nullptr }
	};

}}