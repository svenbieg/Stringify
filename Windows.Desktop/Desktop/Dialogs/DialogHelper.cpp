//==================
// DialogHelper.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "DialogHelper.h"

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
auto filter=String::Create(len+2, nullptr);
auto filter_ptr=const_cast<LPTSTR>(filter->Begin());
for(UINT u=0; u<len; u++)
	{
	if(CharHelper::Compare(filter_str[u], '\n')==0)
		{
		filter_ptr[u]=0;
		}
	else if(CharHelper::Compare(filter_str[u], '|')==0)
		{
		filter_ptr[u]=0;
		}
	else
		{
		filter_ptr[u]=filter_str[u];
		}
	}
filter_ptr[len]=0;
filter_ptr[len+1]=0;
return filter;
}

}}