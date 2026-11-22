//====================
// StringVariable.cpp
//====================

#include "StringVariable.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"

using namespace Storage::Streams;


//========
// Access
//========

Handle<String> StringVariable::Get()
{
Handle<String> value=m_Value;
Reading(this, value);
return value;
}

SIZE_T StringVariable::WriteToStream(OutputStream* stream)
{
auto value=Get();
SIZE_T size=0;
StreamWriter writer(stream);
size+=writer.Print(value);
size+=writer.PrintChar('\0');
return size;
}


//==============
// Modification
//==============

BOOL StringVariable::FromString(Handle<String> value)
{
return Set(value);
}

SIZE_T StringVariable::ReadFromStream(InputStream* stream)
{
if(!stream)
	return 0;
SIZE_T size=0;
StreamReader reader(stream);
auto str=reader.ReadString();
Set(str);
return size;
}

BOOL StringVariable::Set(Handle<String> value)
{
if(m_Value==value)
	return true;
m_Value=value;
Changed(this);
return true;
}
