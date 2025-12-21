//================
// VirtualKey.cpp
//================

#include "UI/Input/VirtualKey.h"


//=======
// Using
//=======

#include "StringHelper.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Input {


//==========
// Char-Map
//==========

constexpr CHAR CharMap[256]=
	{
//    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x00..0x0F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x10..0x1F
	' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x20..0x2F
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',   0,   0,   0,   0,   0,   0, // 0x30..0x3F
	  0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', // 0x40..0x4F
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',   0,   0,   0,   0,   0, // 0x50..0x5F
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',   0,   0,   0,   0,   0,   0, // 0x60..0x6F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x70..0x7F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x80..0x8F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x90..0x9F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xA0..0xAF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 'ü', '+', ',', '-', '.', '#', // 0xB0..0xBF
	'ö',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xC0..0xCF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 'ß', '^',   0, 'ä',   0, // 0xD0..0xDF
	  0,   0, '<',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xE0..0xEF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xF0..0xFF
	};

constexpr CHAR CharMapShift[256]=
	{
//    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x00..0x0F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x10..0x1F
	' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x20..0x2F
	'=', '!',  34,   0, '$', '%', '&', '/', '(', ')',   0,   0,   0,   0,   0,   0, // 0x30..0x3F
	  0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', // 0x40..0x4F
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',   0,   0,   0,   0,   0, // 0x50..0x5F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x60..0x6F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x70..0x7F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x80..0x8F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x90..0x9F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xA0..0xAF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 'Ü', '*', ';', '_', ':',  39, // 0xB0..0xBF
	'Ö',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xC0..0xCF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, '?', '°',   0, 'Ä',   0, // 0xD0..0xDF
	  0,   0, '>',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xE0..0xEF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xF0..0xFF
	};

constexpr CHAR CharMapCtrl[256]=
	{
//    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x00..0x0F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x10..0x1F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x20..0x2F
	'}',   0,   0,   0,   0,   0,   0, '{', '[', ']',   0,   0,   0,   0,   0,   0, // 0x30..0x3F
	  0,   0,   0,   0,   0, '€',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x40..0x4F
	  0, '@',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x50..0x5F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x60..0x6F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x70..0x7F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x80..0x8F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x90..0x9F
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xA0..0xAF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, '~',   0,   0,   0,   0, // 0xB0..0xBF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xC0..0xCF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  92,   0,   0,   0,   0, // 0xD0..0xDF
	  0,   0, '|',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xE0..0xEF
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0xF0..0xFF
	};


//========
// Common
//========

VirtualKey VirtualKeyFromString(LPCSTR str)
{
UINT len=StringHelper::Length(str);
if(!len)
	return VirtualKey::None;
if(len==1)
	return (VirtualKey)CharHelper::ToCapitalAnsi(str[0]);
if(StringHelper::Compare(str, "alt", 0, false)==0)
	return VirtualKey::Alt;
if(StringHelper::Compare(str, "ctrl", 0, false)==0)
	return VirtualKey::Control;
if(StringHelper::Compare(str, "del", 0, false)==0)
	return VirtualKey::Delete;
if(StringHelper::Compare(str, "entf", 0, false)==0)
	return VirtualKey::Delete;
if(StringHelper::Compare(str, "strg", 0, false)==0)
	return VirtualKey::Control;
CHAR c=0;
UINT func=0;
if(StringHelper::Scan(str, "%c%u", &c, &func)==2)
	{
	if(CharHelper::Compare(c, 'F', false)!=0)
		return VirtualKey::None;
	return (VirtualKey)(111+func);
	}
return VirtualKey::None;
}

CHAR VirtualKeyToChar(VirtualKey key, BOOL shift, BOOL ctrl)
{
if(shift&&ctrl)
	return 0;
BYTE u=(BYTE)key;
LPCSTR map=shift? CharMapShift: CharMap;
if(ctrl)
	map=CharMapCtrl;
CHAR c=map[u];
if(!c)
	return 0;
return c;
}

}}