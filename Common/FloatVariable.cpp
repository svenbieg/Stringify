//===================
// FloatVariable.cpp
//===================

#include "FloatVariable.h"


//========
// Access
//========

FLOAT FloatVariable::Get()
{
FLOAT value=m_Value;
Reading(this, value);
return value;
}

SIZE_T FloatVariable::WriteToStream(OutputStream* Stream)
{
if(!Stream)
	return sizeof(FLOAT);
FLOAT value=Get();
return Stream->Write(&value, sizeof(FLOAT));
}


//==============
// Modification
//==============

BOOL FloatVariable::FromString(Handle<String> value)
{
if(!value)
	return false;
FLOAT f;
if(value->Scan("%f", &f)!=1)
	return false;
return Set(f);
}

SIZE_T FloatVariable::ReadFromStream(InputStream* stream)
{
if(!stream)
	return sizeof(FLOAT);
FLOAT value;
SIZE_T size=stream->Read(&value, sizeof(FLOAT));
if(size==sizeof(FLOAT))
	Set(value);
return size;
}

BOOL FloatVariable::Set(FLOAT value)
{
if(m_Value==value)
	return false;
m_Value=value;
Changed(this);
return true;
}
