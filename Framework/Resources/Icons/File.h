//========
// Edit.h
//========

#pragma once


//=======
// Using
//=======

#include "Resources/Bitmaps/File.h"
#include "Resources/Icons/Icon.h"


//===========
// Namespace
//===========

namespace Resources {
	namespace Icons {


//========
// Common
//========

constexpr ICON ICO_FILE_NEW[]=
	{
	{ 16, Bitmaps::BMP_FILE_NEW_16 },
	{ 24, Bitmaps::BMP_FILE_NEW_24 },
	{ 0, nullptr }
	};

constexpr ICON ICO_FILE_OPEN[]=
	{
	{ 16, Bitmaps::BMP_FILE_OPEN_16 },
	{ 24, Bitmaps::BMP_FILE_OPEN_24 },
	{ 0, nullptr }
	};

constexpr ICON ICO_FILE_SAVE[]=
	{
	{ 16, Bitmaps::BMP_FILE_SAVE_16 },
	{ 24, Bitmaps::BMP_FILE_SAVE_24 },
	{ 0, nullptr }
	};

}}