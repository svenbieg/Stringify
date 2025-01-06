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
#include "DispatchedQueue.h"
#include "Signal.h"


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
	static Handle<Task> Create(VOID (*Procedure)());
	template <class _owner_t> static Handle<Task> Create(_owner_t* Owner, VOID (_owner_t::*Procedure)());
	template <class _owner_t> static Handle<Task> Create(Handle<_owner_t> Owner, VOID (_owner_t::*Procedure)());
	template <class _lambda_t> static Handle<Task> Create(nullptr_t Owner, _lambda_t&& Lambda);
	template <class _owner_t, class _lambda_t> static Handle<Task> Create(_owner_t* Owner, _lambda_t&& Lambda);
	static Handle<Task> Get();
	inline HANDLE GetHandle()const { return m_Thread; }
	Handle<Object> GetResult();
	Handle<Object> Result;
	virtual VOID Run()=0;
	static inline VOID SetResult(Handle<Object> Result) { Task::Get()->Result=Result; }
	static VOID Sleep(UINT MilliSeconds);
	static VOID SleepMicroseconds(UINT MicroSeconds);
	inline VOID Then(VOID (*Procedure)())
		{
		assert(m_Then==nullptr);
		m_Then=new DispatchedProcedure(Procedure);
		}
	static inline VOID ThrowIfMain()
		{
		if(Get()==nullptr)
			throw InvalidContextException();
		}
	Status Wait();

protected:
	// Con-/Destructors
	Task();

private:
	// Common
	static VOID RunDeferred(Handle<Task> Task);
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

template <class _lambda_t> class TaskLambda<nullptr_t, _lambda_t>: public Task
{
public:
	// Con-/Destructors
	TaskLambda(nullptr_t Owner, _lambda_t&& Lambda):
		m_Lambda(std::move(Lambda))
		{}

	// Common
	VOID Run()override { m_Lambda(); }

private:
	// Common
	_lambda_t m_Lambda;
};


//==================
// Con-/Destructors
//==================

template <class _owner_t> inline Handle<Task> Task::Create(_owner_t* Owner, VOID (_owner_t::*Procedure)())
{
Handle<Task> task=new TaskMemberProcedure(Owner, Procedure);
RunDeferred(task);
return task;
}

template <class _owner_t> inline Handle<Task> Task::Create(Handle<_owner_t> Owner, VOID (_owner_t::*Procedure)())
{
Handle<Task> task=new TaskMemberProcedure(Owner, Procedure);
RunDeferred(task);
return task;
}

template <class _lambda_t> inline Handle<Task> Task::Create(nullptr_t Owner, _lambda_t&& Lambda)
{
Handle<Task> task=new TaskLambda<nullptr_t, _lambda_t>(nullptr, std::forward<_lambda_t>(Lambda));
RunDeferred(task);
return task;
}

template <class _owner_t, class _lambda_t> inline Handle<Task> Task::Create(_owner_t* Owner, _lambda_t&& Lambda)
{
Handle<Task> task=new TaskLambda<_owner_t, _lambda_t>(Owner, std::forward<_lambda_t>(Lambda));
RunDeferred(task);
return task;
}

}