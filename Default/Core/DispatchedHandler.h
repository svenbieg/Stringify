//=====================
// DispatchedHandler.h
//=====================

#pragma once


//=======
// Using
//=======

#include <functional>


//===========
// Namespace
//===========

namespace Core {


//====================
// Dispatched-Handler
//====================

class DispatchedHandler: public Object
{
public:
	// Con-/Destructors
	virtual ~DispatchedHandler() {}

	// Common
	VOID Abort()
		{
		ScopedLock lock(cMutex);
		cDispatched.Broadcast();
		}
	virtual VOID Run()=0;
	VOID Wait()
		{
		ScopedLock lock(cMutex);
		cDispatched.Wait(lock);
		}

protected:
	// Con-/Destructors
	DispatchedHandler() {}

	// Common
	Signal cDispatched;
	Mutex cMutex;
};


//==========================
// Dispatched-Handler-Typed
//==========================

template <class _receiver_t, class... _args_t>
class DispatchedHandlerTyped: public DispatchedHandler
{
public:
	// Procedure
	typedef VOID (_receiver_t::*DispatchedProc)(_args_t...);

	// Con-/Destructors
	DispatchedHandlerTyped(_receiver_t* Receiver, DispatchedProc Procedure, _args_t... Arguments):
		cProcedure([Receiver, Procedure, Arguments...]() { (Receiver->*Procedure)(Arguments...); }),
		hReceiver(Receiver) {}

	// Common
	VOID Run()override
		{
		cProcedure();
		ScopedLock lock(cMutex);
		cDispatched.Broadcast();
		}

private:
	// Common
	std::function<VOID()> cProcedure;
	Handle<_receiver_t> hReceiver;
};

}