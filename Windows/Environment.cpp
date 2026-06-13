//=================
// Environment.cpp
//=================

#include "Environment.h"


//========
// Common
//========

VOID Environment::Open(Handle<String> path)
{
ShellExecute(NULL, TEXT("open"), path->Begin(), nullptr, nullptr, SW_SHOWNORMAL);
}
