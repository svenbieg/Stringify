//===============
// CommandLine.h
//===============

#pragma once


//==============
// Command-Line
//==============

class CommandLine: public Object
{
public:
	// Con-/Destructors
	CommandLine(LPSTR CommandLine);
	~CommandLine();

	// Common
	static Handle<CommandLine> Current;
	LPCSTR GetArgument(UINT Id)const;
	UINT GetArgumentCount()const { return uCount; }
	BOOL HasArgument(LPCTSTR Argument);

private:
	// Common
	LPSTR ScanArgument(LPSTR CommandLine, LPSTR* Argument, UINT* Length);
	LPSTR* ppArgs;
	UINT uCount;
};
