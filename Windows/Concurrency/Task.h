//========
// Task.h
//========

#pragma once


//=======
// Using
//=======

#include <assert.h>
#include <utility>
#include "Collections/shared_map.hpp"
#include "Concurrency/DispatchedQueue.h"


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
	~Task();

	// Common
	VOID Cancel();
	volatile BOOL Cancelled;
	static Handle<Task> Get();
	inline HANDLE GetHandle()const { return m_Thread; }
	VOID Resume();
	virtual VOID Run()=0;
	inline VOID Then(VOID (*Procedure)())
		{
		assert(m_Then==nullptr);
		m_Then=new DispatchedProcedure(Procedure);
		}
	VOID Wait();

protected:
	// Con-/Destructors
	Task();

private:
	// Common
	static DWORD WINAPI TaskProc(VOID* Param);
	Signal m_Done;
	DWORD m_Id;
	Mutex m_Mutex;
	Status m_Status;
	DispatchedHandler* m_Then;
	Handle<Task> m_This;
	HANDLE m_Thread;
	static Collections::shared_map<DWORD, Task*> s_Tasks;
};

class TaskProcedure: public Task
{
public:
	// Using
	typedef VOID(*TASK_PROC)();

	// Con-/Destructors
	TaskProcedure(TASK_PROC Procedure): m_Procedure(Procedure) {}

	// Common
	VOID Run()override { m_Procedure(); }

private:
	// Common
	TASK_PROC m_Procedure;
};

template <class _owner_t> class TaskMemberProcedure: public Task
{
public:
	// Using
	typedef VOID(_owner_t::*TASK_PROC)();

	// Con-/Destructors
	TaskMemberProcedure(_owner_t* Owner, TASK_PROC Procedure):
		m_Owner(Owner),
		m_Procedure(Procedure)
		{}

	// Common
	VOID Run()override { (m_Owner->*m_Procedure)(); }

private:
	// Common
	Handle<_owner_t> m_Owner;
	TASK_PROC m_Procedure;
};

template <class _owner_t, class _lambda_t> class TaskLambda: public Task
{
public:
	// Con-/Destructors
	TaskLambda(Handle<_owner_t> Owner, _lambda_t&& Lambda):
		m_Lambda(std::move(Lambda)),
		m_Owner(Owner)
		{}

	// Common
	VOID Run()override { m_Lambda(); }

private:
	// Common
	_lambda_t m_Lambda;
	Handle<_owner_t> m_Owner;
};

}
