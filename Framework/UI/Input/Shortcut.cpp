//==============
// Shortcut.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "UI/Input/VirtualKey.h"
#include "Shortcut.h"

using namespace UI::Input;


//===========
// Namespace
//===========

namespace UI {
	namespace Input {


//========
// Common
//========

UINT ShortcutFromString(Handle<String> text)
{
if(!text)
	return 0;
auto shortcut=text->Begin();
CHAR str1[8];
CHAR str2[8];
CHAR str3[8];
UINT count=StringHelper::Scan(shortcut, "%s+%s+%s", str1, 8, str2, 8, str3, 8);
if(count==0)
	return 0;
UINT code=0;
ShortcutFlags flags=ShortcutFlags::None;
LPCSTR str[3]={ str1, str2, str3 };
for(UINT u=0; u<count; u++)
	{
	VirtualKey key=VirtualKeyFromString(str[u]);
	switch(key)
		{
		case VirtualKey::Alt:
			{
			SetFlag(flags, ShortcutFlags::Alt);
			continue;
			}
		case VirtualKey::Control:
			{
			SetFlag(flags, ShortcutFlags::Ctrl);
			continue;
			}
		case VirtualKey::Shift:
			{
			SetFlag(flags, ShortcutFlags::Alt);
			continue;
			}
		default:
			{
			code=(UINT)key;
			break;
			}
		}
	}
if(!code)
	return 0;
return code|(UINT)flags;
}

}}