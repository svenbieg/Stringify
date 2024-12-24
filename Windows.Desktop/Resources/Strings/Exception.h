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
	{ LNG::DE, L"Unbehandelte Ausnahme" },
	{ LNG::EN, L"Unhandled Exception" },
	{ LNG::None, nullptr }
	};

}}