//=========
// Files.h
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


//=======
// Files
//=======

constexpr STRING STR_FILE_OPEN[]=
	{
	{ LNG::DE, "Datei öffnen" },
	{ LNG::EN, "Open file" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_FILE_SAVE[]=
	{
	{ LNG::DE, "Datei speichern" },
	{ LNG::EN, "Save file" },
	{ LNG::None, nullptr }
	};

constexpr STRING STR_FILTER_ALL[]=
	{
	{ LNG::DE, "Alle Dateien (*.*)|*.*" },
	{ LNG::EN, "All files (*.*)|*.*" },
	{ LNG::None, nullptr }
	};

}}