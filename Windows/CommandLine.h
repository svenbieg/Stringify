//===============
// CommandLine.h
//===============

#pragma once


//=======
// Using
//=======

#include "Collections/StringList.h"


//==============
// Command-Line
//==============

class CommandLine: public Object
{
public:
	// Using
	using StringList=Collections::StringList;

	// Con-/Destructors
	CommandLine(LPTSTR CommandLine);

	// Common
	Handle<StringList> Arguments;
	LPCTSTR Begin();
	static Handle<CommandLine> Get();

private:
	// Common
	LPCTSTR ScanArgument(LPCTSTR CommandLine, LPCTSTR* Argument, UINT* Length);
	static Handle<CommandLine> m_Current;
};
