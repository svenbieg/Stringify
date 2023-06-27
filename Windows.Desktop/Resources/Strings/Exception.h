//=============
// Exception.h
//=============

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
// Errors
//========

constexpr STRING STR_EXCEPTION[]=
	{
	{ LNG::DE, "Unbehandelte Ausnahme" },
	{ LNG::EN, "Unhandled Exception" },
	{ LNG::None, nullptr }
	};

}}