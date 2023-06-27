//===============
// Application.h
//===============

#pragma once


//=======
// Using
//=======

#include "Clusters/shared_list.hpp"
#include "DispatchedHandler.h"


//===========
// Namespace
//===========

namespace Core {


//=============
// Application
//=============

class Application: public Object
{
public:
	// Using
	using DispatchedHandler=Core::DispatchedHandler;

	// Common
	static Handle<Application> Current;
	template <class _receiver_t, class... _args_t> Handle<DispatchedHandler> Dispatch(_receiver_t* Receiver, VOID (_receiver_t::*Procedure)(_args_t...), _args_t... Arguments)
		{
		if(!Running)
			return nullptr;
		Handle<DispatchedHandler> handler=new DispatchedHandlerTyped<_receiver_t, _args_t...>(Receiver, Procedure, Arguments...);
		cDispatchedHandlers.append(handler);
		Dispatched(this);
		return handler;
		}
	template <class _receiver_t, class... _args_t> inline Handle<DispatchedHandler> Dispatch(Handle<_receiver_t> Receiver, VOID (_receiver_t::*Procedure)(_args_t...), _args_t... Arguments)
		{
		return Dispatch((_receiver_t*)Receiver, Procedure, Arguments...);
		}
	Event<Application> Dispatched;
	VOID HandleDispatched();
	LPCSTR Name;
	volatile BOOL Running;
	VOID Quit();
	LPCSTR Version;

protected:
	// Con-/Destructors
	Application(LPCSTR Name, LPCSTR Version="1.0");

	// Common
	Clusters::shared_list<Handle<DispatchedHandler>> cDispatchedHandlers;
};

}