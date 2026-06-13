//===============
// CommandLine.h
//===============

#pragma once


//=======
// Using
//=======

#include "Collections/StringList.h"
#include "Global.h"


//==============
// Command-Line
//==============

class CommandLine: public Global<CommandLine>
{
public:
	// Using
	using StringList=Collections::StringList;

	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<CommandLine> Create() { return Global::Create(); }

	// Common
	Handle<StringList> Arguments;
	LPCTSTR Begin();

private:
	// Con-/Destructors
	CommandLine();

	// Common
	LPCTSTR ScanArgument(LPCTSTR CommandLine, LPCTSTR* Argument, UINT* Length);
};
