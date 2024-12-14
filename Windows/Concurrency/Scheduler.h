//=============
// Scheduler.h
//=============

#pragma once


//=======
// Using
//=======

#include "MainTask.h"
#include "Task.h"


//===========
// Namespace
//===========

namespace Concurrency {


//===========
// Scheduler
//===========

class Scheduler
{
public:
	// Common
	static inline Handle<Task> CreateTask(VOID (*Procedure)())
		{
		Handle<Task> task=new TaskProcedure(Procedure);
		MainTask::Dispatch(task, [task](){ task->Resume(); });
		return task;
		}
	template <class _owner_t> static inline Handle<Task> CreateTask(_owner_t* Owner, VOID (_owner_t::*Procedure)())
		{
		Handle<Task> task=new TaskMemberProcedure<_owner_t>(Owner, Procedure);
		MainTask::Dispatch(task, [task](){ task->Resume(); });
		return task;
		}
	template <class _owner_t, class... _args_t> static inline Handle<Task> CreateTask(Handle<_owner_t> Owner, VOID (_owner_t::*Procedure)(_args_t...), _args_t... Arguments)
		{
		Handle<Task> task=new TaskMemberProcedure<_owner_t>(Owner, Procedure);
		MainTask::Dispatch(task, [task](){ task->Resume(); });
		return task;
		}
	template <class _owner_t, class _lambda_t> static inline Handle<Task> CreateTask(_owner_t* Owner, _lambda_t&& Lambda)
		{
		Handle<Task> task=new TaskLambda<_owner_t, _lambda_t>(Owner, std::forward<_lambda_t>(Lambda));
		MainTask::Dispatch(task, [task](){ task->Resume(); });
		return task;
		}
};

}