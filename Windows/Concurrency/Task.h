//========
// Task.h
//========

#pragma once


//=======
// Using
//=======

#include "Clusters/shared_map.hpp"


//===========
// Namespace
//===========

namespace Concurrency {


//======
// Task
//======

class Task: public Object
{
public:
	// Con-/Destructors
	Task(std::function<VOID()> Procedure);
	~Task();

	// Common
	VOID Cancel();
	volatile BOOL Cancelled;
	inline HANDLE GetHandle() { return hThread; }
	static Task* GetTask(DWORD Id);
	VOID Wait();

private:
	// Common
	VOID DoTask();
	static DWORD WINAPI TaskProc(VOID* Param);
	Signal cFinished;
	Mutex cMutex;
	std::function<VOID()> cProcedure;
	static Clusters::shared_map<DWORD, Task*> cTasks;
	HANDLE hThread;
	DWORD uId;
};

}
