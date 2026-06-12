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

const STRING STR_FILE_FILTER_ALL[]=
	{
	{ LNG::DE, L"Alle Dateien (*.*)|*.*" },
	{ LNG::EN, L"All files (*.*)|*.*" },
	{ LNG::None, nullptr }
	};

const STRING STR_FILE_OPEN[]=
	{
	{ LNG::DE, L"Datei öffnen" },
	{ LNG::EN, L"Open file" },
	{ LNG::None, nullptr }
	};

const STRING STR_FILE_SAVE[]=
	{
	{ LNG::DE, L"Datei speichern" },
	{ LNG::EN, L"Save file" },
	{ LNG::None, nullptr }
	};

}}