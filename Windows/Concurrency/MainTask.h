//============
// MainTask.h
//============

#pragma once


//=======
// Using
//=======

#include "DispatchedQueue.h"
#include "Task.h"


//===========
// Namespace
//===========

namespace Concurrency {


//===========
// Main-Task
//===========

class MainTask
{
public:
	// Common
	static inline VOID Dispatch(DispatchedHandler* Handler)
		{
		DispatchedQueue::Append(Handler);
		PostThreadMessage(s_ThreadId, WM_DISPATCH, 0, 0);
		}
	static inline VOID Dispatch(VOID (*Procedure)())
		{
		auto handler=new Concurrency::DispatchedProcedure(Procedure);
		DispatchedQueue::Append(handler);
		PostThreadMessage(s_ThreadId, WM_DISPATCH, 0, 0);
		}
	template <class _owner_t> static inline VOID Dispatch(_owner_t* Owner, VOID (_owner_t::*Procedure)())
		{
		auto handler=new Concurrency::DispatchedMemberProcedure<_owner_t>(Owner, Procedure);
		DispatchedQueue::Append(handler);
		PostThreadMessage(s_ThreadId, WM_DISPATCH, 0, 0);
		}
	template <class _owner_t, class _lambda_t> static inline VOID Dispatch(_owner_t* Owner, _lambda_t&& Lambda)
		{
		auto handler=new Concurrency::DispatchedLambda<_owner_t, _lambda_t>(Owner, std::forward<_lambda_t>(Lambda));
		DispatchedQueue::Append(handler);
		PostThreadMessage(s_ThreadId, WM_DISPATCH, 0, 0);
		}
	template <class _owner_t, class _lambda_t> static inline VOID Dispatch(Handle<_owner_t> Owner, _lambda_t&& Lambda)
		{
		auto handler=new Concurrency::DispatchedLambda<_owner_t, _lambda_t>(Owner, std::forward<_lambda_t>(Lambda));
		DispatchedQueue::Append(handler);
		PostThreadMessage(s_ThreadId, WM_DISPATCH, 0, 0);
		}
	static VOID Initialize();
	static inline VOID ThrowIf()
		{
		if(Task::Get()==nullptr)
			throw InvalidContextException();
		}
	static inline VOID ThrowIfNot()
		{
		if(Task::Get())
			throw InvalidContextException();
		}

private:
	// Common
	static DWORD s_ThreadId;
};

}