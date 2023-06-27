//========
// Icon.h
//========

#pragma once


//========
// Common
//========

#ifdef RC_INVOKED
#define ICON(Name, Id, Path) Id ICON Path
#define ICONS_BEGIN
#define ICONS_END
#else
#define ICON(Name, Id, Path) constexpr UINT Name=Id;
#define ICONS_BEGIN namespace Resources { namespace Icons {
#define ICONS_END }}
#endif
