//==================
// DialogHelper.cpp
//==================

#include "Desktop/Dialogs/DialogHelper.h"


//=======
// Using
//=======

#include "StringBuilder.h"

using namespace Culture;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Common
//========

Handle<String> FilterFromSentence(Handle<Sentence> sentence)
{
if(!sentence)
	return nullptr;
auto filter_str=sentence->Begin();
UINT len=StringHelper::Length(filter_str);
StringBuilder builder(len+2);
for(UINT u=0; u<len; u++)
	{
	if(CharHelper::Compare(filter_str[u], '\n')==0)
		{
		builder.Append('\0');
		}
	else if(CharHelper::Compare(filter_str[u], '|')==0)
		{
		builder.Append('\0');
		}
	else
		{
		builder.Append(filter_str[u]);
		}
	}
builder.Append('\0');
builder.Append('\0');
return builder.ToString();
}

}}