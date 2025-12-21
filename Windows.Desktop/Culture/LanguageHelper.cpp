//====================
// LanguageHelper.cpp
//====================

#include "Culture/LanguageHelper.h"


//===========
// Namespace
//===========

namespace Culture {


//========
// Common
//========

LanguageCode GetCurrentLanguage()
{
LANGID id=GetUserDefaultUILanguage();
BYTE lng=(BYTE)id;
switch(lng)
	{
	case LANG_GERMAN:
		return LanguageCode::DE;
	case LANG_ENGLISH:
		return LanguageCode::EN;
	default:
		break;
	}
return LanguageCode::None;
}

}