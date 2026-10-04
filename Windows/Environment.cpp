//=================
// Environment.cpp
//=================

#include "Environment.h"


//=======
// Using
//=======

#pragma comment(lib, "shell32.lib")


//========
// Common
//========

VOID Environment::Open(Handle<String> path)
{
ShellExecute(NULL, TEXT("open"), path->Begin(), nullptr, nullptr, SW_SHOWNORMAL);
}
