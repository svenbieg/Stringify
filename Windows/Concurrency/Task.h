//========
// Task.h
//========

#pragma once


//=======
// Using
//=======

#include "Collections/shared_map.hpp"


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
	inline HANDLE GetHandle() { return m_Thread; }
	static Handle<Task> GetTask(DWORD Id);
	VOID Wait();

private:
	// Common
	VOID DoTask();
	static DWORD WINAPI TaskProc(VOID* Param);
	Signal m_Done;
	DWORD s_Id;
	Mutex m_Mutex;
	std::function<VOID()> m_Procedure;
	Handle<Task> m_This;
	HANDLE m_Thread;
	static Collections::shared_map<DWORD, Task*> s_Tasks;
};

}
