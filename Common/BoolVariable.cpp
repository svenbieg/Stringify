//==================
// BoolVariable.cpp
//==================

#include "BoolVariable.h"


//=======
// Using
//=======

#include "Resources/Strings/Bool.h"

using namespace Resources::Strings;


//========
// Access
//========

BOOL BoolVariable::Get()
{
BOOL value=m_Value;
Reading(this, value);
return value;
}

SIZE_T BoolVariable::WriteToStream(OutputStream* Stream)
{
if(!Stream)
	return sizeof(BOOL);
BOOL value=Get();
return Stream->Write(&value, sizeof(BOOL));
}


//==============
// Modification
//==============

BOOL BoolVariable::FromString(Handle<String> str)
{
BOOL value;
if(FromString(str, &value))
	{
	Set(value);
	return true;
	}
return false;
}

BOOL BoolVariable::FromString(Handle<String> str, BOOL* value_ptr)
{
if(!str)
	return false;
for(UINT u=0; u<TypeHelper::ArraySize(STR_TRUE); u++)
	{
	if(StringHelper::Compare(str, STR_TRUE[u].Value, false)==0)
		{
		if(value_ptr)
			*value_ptr=true;
		return true;
		}
	}
for(UINT u=0; u<TypeHelper::ArraySize(STR_FALSE); u++)
	{
	if(StringHelper::Compare(str, STR_FALSE[u].Value, false)==0)
		{
		if(value_ptr)
			*value_ptr=false;
		return true;
		}
	}
return false;
}

SIZE_T BoolVariable::ReadFromStream(InputStream* stream)
{
if(!stream)
	return sizeof(BOOL);
BOOL value;
SIZE_T size=stream->Read(&value, sizeof(BOOL));
if(size==sizeof(BOOL))
	Set(value);
return size;
}

BOOL BoolVariable::Set(BOOL value)
{
if(m_Value==value)
	return false;
m_Value=value;
Changed(this);
return true;
}
