//========
// Edit.h
//========

#pragma once


//=======
// Using
//=======

#include "Resources/Bitmaps/Edit.h"
#include "Resources/Icons/Icon.h"


//===========
// Namespace
//===========

namespace Resources {
	namespace Icons {


//========
// Common
//========

constexpr ICON ICO_EDIT_COPY[]=
	{
	{ 16, Bitmaps::BMP_EDIT_COPY_16 },
	{ 24, Bitmaps::BMP_EDIT_COPY_24 },
	{ 0, nullptr }
	};

constexpr ICON ICO_EDIT_CUT[]=
	{
	{ 16, Bitmaps::BMP_EDIT_CUT_16 },
	{ 24, Bitmaps::BMP_EDIT_CUT_24 },
	{ 0, nullptr }
	};

constexpr ICON ICO_EDIT_DELETE[]=
	{
	{ 16, Bitmaps::BMP_EDIT_DELETE_16 },
	{ 24, Bitmaps::BMP_EDIT_DELETE_24 },
	{ 0, nullptr }
	};

constexpr ICON ICO_EDIT_PASTE[]=
	{
	{ 16, Bitmaps::BMP_EDIT_PASTE_16 },
	{ 24, Bitmaps::BMP_EDIT_PASTE_24 },
	{ 0, nullptr }
	};

constexpr ICON ICO_EDIT_SELECT_ALL[]=
	{
	{ 16, Bitmaps::BMP_EDIT_SELECT_ALL_16 },
	{ 24, Bitmaps::BMP_EDIT_SELECT_ALL_24 },
	{ 0, nullptr }
	};

}}