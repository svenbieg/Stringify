//=====================
// ExceptionHelper.cpp
//=====================

#include "ExceptionHelper.h"


//=======
// Using
//=======

#pragma comment(lib, "Dbghelp.lib")

#include <DbgHelp.h>
#include <TlHelp32.h>
#include "StringHelper.h"

constexpr UINT MAX_SYM_NAME_LEN=128;


//========
// Common
//========

BOOL LoadSymbols()
{
HANDLE proc=GetCurrentProcess();
if(!SymInitialize(proc, 0, false))
	return false;
if(!SymSetOptions(SYMOPT_INCLUDE_32BIT_MODULES))
	return false;
SetLastError(0);
HANDLE snap_shot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
if(snap_shot==INVALID_HANDLE_VALUE)
	snap_shot=NULL;
if(!snap_shot)
	return false;
MODULEENTRY32 mod={ 0 };
mod.dwSize=sizeof(MODULEENTRY32);
if(!Module32First(snap_shot, &mod))
	{
	CloseHandle(snap_shot);
	return false;
	}
while(mod.hModule)
	{
	#ifdef _UNICODE
	CHAR str[MAX_PATH];
	StringHelper::Copy(str, MAX_PATH, mod.szModule);
	#else
	LPSTR str=mod.szModule;
	#endif
	SymLoadModule64(proc, 0, str, 0, (UINT64)mod.modBaseAddr, mod.modBaseSize);
	if(!Module32Next(snap_shot, &mod))
		break;
	}
CloseHandle(snap_shot);
return true;
}

UINT PrintExceptionContext(CONTEXT* pc, UINT levels, LPSTR str, UINT size)
{
if(!str||!size)
	return 0;
str[0]=0;
if(!LoadSymbols())
	return 0;
HANDLE proc=GetCurrentProcess();
HANDLE thread=GetCurrentThread();
CONTEXT context;
MemoryHelper::Copy(&context, pc, sizeof(CONTEXT));
STACKFRAME64 sf={ 0 };
#ifdef _ARM_
DWORD dwarch=IMAGE_FILE_MACHINE_ARM;
sf.AddrFrame.Offset=0;
sf.AddrFrame.Mode=AddrModeFlat;
sf.AddrPC.Offset=0;
sf.AddrPC.Mode=AddrModeFlat;
sf.AddrStack.Offset=context.Sp;
sf.AddrStack.Mode=AddrModeFlat;
DebugBreak();
#endif
#ifdef _X86_
DWORD dwarch=IMAGE_FILE_MACHINE_I386;
sf.AddrFrame.Offset=context.Ebp;
sf.AddrFrame.Mode=AddrModeFlat;
sf.AddrPC.Offset=context.Eip;
sf.AddrPC.Mode=AddrModeFlat;
sf.AddrStack.Offset=context.Esp;
sf.AddrStack.Mode=AddrModeFlat;
#endif
#ifdef _AMD64_
DWORD dwarch=IMAGE_FILE_MACHINE_AMD64;
sf.AddrFrame.Offset=context.Rsp;
sf.AddrFrame.Mode=AddrModeFlat;
sf.AddrPC.Offset=context.Rip;
sf.AddrPC.Mode=AddrModeFlat;
sf.AddrStack.Offset=context.Rsp;
sf.AddrStack.Mode=AddrModeFlat;
#endif
auto sym_info=(SYMBOL_INFO*)operator new(sizeof(SYMBOL_INFO)+MAX_SYM_NAME_LEN);
MemoryHelper::Fill(sym_info, sizeof(SYMBOL_INFO), 0);
sym_info->SizeOfStruct=sizeof(SYMBOL_INFO);
sym_info->MaxNameLen=MAX_SYM_NAME_LEN;
UINT len=0;
for(UINT frame=0; frame<levels; frame++)
	{
	if(!StackWalk64(dwarch, proc, thread, &sf, &context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0))
		break;
	if(sf.AddrPC.Offset==sf.AddrReturn.Offset)
		break;
	if(!SymFromAddr(proc, sf.AddrPC.Offset, 0, sym_info))
		continue;
	len+=StringHelper::Copy(&str[len], size-len-2, sym_info->Name);
	str[len++]='\n';
	str[len]=0;
	if(len>=size)
		break;
	}
operator delete(sym_info);
return len;
}
