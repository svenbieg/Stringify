//===========
// FlagsVariable.cpp
//===========

#include "FlagsVariable.h"


//=======
// Using
//=======

#include "Collections/StringList.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"

using namespace Collections;
using namespace Storage::Streams;


//========
// Access
//========

Handle<String> FlagsVariable::ToString(LanguageCode lng)
{
Handle<String> flags;
UINT count=0;
for(auto it=m_Flags.cbegin(); it.has_current(); it.move_next())
	{
	if(count>0)
		flags+="|";
	auto flag=it.get_current();
	flags+=flag;
	count++;
	}
return flags;
}

SIZE_T FlagsVariable::WriteToStream(OutputStream* stream)
{
auto flags=ToString();
SIZE_T size=0;
StreamWriter writer(stream);
size+=writer.Print(flags);
size+=writer.PrintChar('\0');
return size;
}


//==============
// Modification
//==============

VOID FlagsVariable::Clear()
{
if(m_Flags.get_count()==0)
	return;
m_Flags.clear();
Changed(this);
}

VOID FlagsVariable::Clear(Handle<String> flag)
{
if(m_Flags.remove(flag))
	Changed(this);
}

BOOL FlagsVariable::FromString(Handle<String> str)
{
if(!str)
	return false;
if(str=="0")
	{
	Clear();
	return true;
	}
auto list=StringList::Create(str);
BOOL changed=false;
for(auto it=list->Begin(); it->HasCurrent(); it->MoveNext())
	{
	auto flag=it->GetCurrent();
	auto p=flag->Begin();
	if(CharHelper::Equal(p[0], '!'))
		{
		auto clear=String::Create(&p[1]);
		if(m_Flags.contains(clear))
			{
			m_Flags.remove(clear);
			changed=true;
			}
		}
	else
		{
		if(!m_Flags.contains(flag))
			{
			m_Flags.set(flag);
			changed=true;
			}
		}
	}
if(changed)
	Changed(this);
return true;
}

SIZE_T FlagsVariable::ReadFromStream(InputStream* stream)
{
SIZE_T size=0;
StreamReader reader(stream);
auto str=reader.ReadString(&size);
FromString(str);
return size;
}

VOID FlagsVariable::Set(Handle<String> flag)
{
if(m_Flags.contains(flag))
	return;
m_Flags.set(flag);
Changed(this);
}
