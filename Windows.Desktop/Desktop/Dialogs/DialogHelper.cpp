//==================
// DialogHelper.cpp
//==================

#include "DialogHelper.h"


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

LPCTSTR DialogHelper::ExtensionFromFilter(Handle<String> filter, UINT id)
{
auto str=filter->Begin();
UINT pos=0;
while(id>0)
	{
	UINT len=StringHelper::Length(&str[pos]);
	pos+=len+1;
	len=StringHelper::Length(&str[pos]);
	pos+=len+1;
	id--;
	}
UINT len=StringHelper::Length(&str[pos]);
pos+=len+3;
return &str[pos];
}

Handle<String> DialogHelper::FilterFromSentence(Handle<Sentence> sentence)
{
if(!sentence)
	return nullptr;
auto filter_str=sentence->Begin();
UINT len=StringHelper::Length(filter_str);
StringBuilder builder(len+2);
for(UINT u=0; u<len; u++)
	{
	if(CharHelper::Equal(filter_str[u], '|'))
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