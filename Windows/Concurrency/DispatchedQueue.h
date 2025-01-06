//===================
// DispatchedQueue.h
//===================

#pragma once


//=======
// Using
//=======

#include "DispatchedHandler.h"
#include "Signal.h"


//======================
// Forward-Declarations
//======================

namespace Console
{
class Application;
}

namespace Desktop
{
class Application;
}


//===========
// Namespace
//===========

namespace Concurrency {


//==================
// Dispatched-Queue
//==================

class DispatchedQueue
{
public:
	// Friends
	friend Console::Application;
	friend Desktop::Application;

	// Common
	static VOID Append(DispatchedHandler* Handler);
	static inline VOID Append(VOID (*Procedure)())
		{
		auto handler=new Concurrency::DispatchedProcedure(Procedure);
		DispatchedQueue::Append(handler);
		}
	template <class _owner_t> static inline VOID Append(_owner_t* Owner, VOID (_owner_t::*Procedure)())
		{
		auto handler=new Concurrency::DispatchedMemberProcedure<_owner_t>(Owner, Procedure);
		DispatchedQueue::Append(handler);
		PostThreadMessage(s_ThreadId, WM_DISPATCH, 0, 0);
		}
	template <class _owner_t, class _lambda_t> static inline VOID Append(_owner_t* Owner, _lambda_t&& Lambda)
		{
		auto handler=new Concurrency::DispatchedLambda<_owner_t, _lambda_t>(Owner, std::forward<_lambda_t>(Lambda));
		DispatchedQueue::Append(handler);
		PostThreadMessage(s_ThreadId, WM_DISPATCH, 0, 0);
		}
	template <class _owner_t, class _lambda_t> static inline VOID Append(Handle<_owner_t> Owner, _lambda_t&& Lambda)
		{
		auto handler=new Concurrency::DispatchedLambda<_owner_t, _lambda_t>(Owner, std::forward<_lambda_t>(Lambda));
		DispatchedQueue::Append(handler);
		PostThreadMessage(s_ThreadId, WM_DISPATCH, 0, 0);
		}
	static VOID Exit();
	static VOID Initialize();

private:
	// Common
	static VOID Run();
	static DispatchedHandler* s_First;
	static DispatchedHandler* s_Last;
	static Mutex s_Mutex;
	static DWORD s_ThreadId;
};

}