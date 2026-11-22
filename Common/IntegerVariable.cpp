//=====================
// IntegerVariable.cpp
//=====================

#include "IntegerVariable.h"


//========
// Access
//========

INT IntegerVariable::Get()
{
INT value=m_Value;
Reading(this, value);
return value;
}

SIZE_T IntegerVariable::WriteToStream(OutputStream* Stream)
{
if(!Stream)
	return sizeof(INT);
INT value=Get();
return Stream->Write(&value, sizeof(INT));
}


//==============
// Modification
//==============

BOOL IntegerVariable::FromString(Handle<String> str)
{
INT value;
if(FromString(str, &value))
	{
	Set(value);
	return true;
	}
return false;
}

BOOL IntegerVariable::FromString(Handle<String> str, INT* value_ptr)
{
if(!str)
	return false;
INT value;
if(str->Scan("%i", &value)!=1)
	return false;
if(value_ptr)
	*value_ptr=value;
return true;
}

SIZE_T IntegerVariable::ReadFromStream(InputStream* stream)
{
if(!stream)
	return sizeof(INT);
INT value;
SIZE_T size=stream->Read(&value, sizeof(INT));
if(size==sizeof(INT))
	Set(value);
return size;
}

BOOL IntegerVariable::Set(INT value)
{
if(m_Value==value)
	return false;
m_Value=value;
Changed(this);
return true;
}
