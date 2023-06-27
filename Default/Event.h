//=========
// Event.h
//=========

#pragma once


//=======
// Using
//=======

#include "EventHandler.h"
#include "ListHelper.h"


//============
// Event-Base
//============

namespace Details {

class EventBase
{
public:
	// Common
	virtual VOID Remove(VOID* Receiver)=0;
};

template <class _sender_t, class... _args_t>
class EventBaseTyped: public EventBase
{
public:
	// Using
	using _handler_t=EventHandler<_sender_t, _args_t...>;

	// Common
	VOID Remove(VOID* Receiver)override
		{
		_handler_t* handler=pFirst;
		while(handler)
			{
			_handler_t* next=handler->GetNext();
			if(handler->GetReceiver()==Receiver)
				{
				if(handler->IsRunning())
					{
					handler->Invalidate();
					}
				else
					{
					ListRemove<_handler_t>(&pFirst, nullptr, handler);
					}
				}
			handler=next;
			}
		}

protected:
	// Con-/Destructors
	EventBaseTyped(): pFirst(nullptr) {}

	// Common
	VOID RunInternal(_sender_t* Sender, _args_t... Arguments)
		{
		_handler_t* handler=pFirst;
		while(handler)
			{
			handler->Run(Sender, Arguments...);
			_handler_t* next=handler->GetNext();
			if(!handler->IsValid())
				ListRemove<_handler_t>(&pFirst, nullptr, handler);
			handler=next;
			}
		}
	_handler_t* pFirst;
};

}


//=======
// Event
//=======

template <class _sender_t, class... _args_t>
class Event: public Details::EventBaseTyped<_sender_t, _args_t...>
{
public:
	// Using
	using _handler_t=Details::EventHandler<_sender_t, _args_t...>;

	// Con-/Destructors
	Event() {}
	Event(Event const&)=delete;
	Event(Event&&)=delete;

	// Access
	inline operator BOOL() { return this->pFirst!=nullptr; }
	VOID operator()(_sender_t* Sender, _args_t... Arguments) { this->RunInternal(Sender, Arguments...); }

	// Modification
	inline VOID Add(VOID (*Procedure)())
		{
		_handler_t* handler=new Details::EventHandlerProcedure<_sender_t, _args_t...>(Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(_receiver_t* Receiver, VOID (_receiver_t::*Procedure)())
		{
		_handler_t* handler=new Details::EventHandlerWithReceiver<_sender_t, _receiver_t, _args_t...>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(Handle<_receiver_t> Receiver, VOID (_receiver_t::*Procedure)())
		{
		_handler_t* handler=new Details::EventHandlerWithReceiver<_sender_t, _receiver_t, _args_t...>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(_receiver_t* Receiver, VOID (_receiver_t::*Procedure)(_args_t...))
		{
		_handler_t* handler=new Details::EventHandlerWithArgs<_sender_t, _receiver_t, _args_t...>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(Handle<_receiver_t> Receiver, VOID (_receiver_t::*Procedure)(_args_t...))
		{
		_handler_t* handler=new Details::EventHandlerWithArgs<_sender_t, _receiver_t, _args_t...>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(_receiver_t* Receiver, VOID (_receiver_t::*Procedure)(_sender_t*, _args_t...))
		{
		_handler_t* handler=new Details::EventHandlerWithSender<_sender_t, _receiver_t, _args_t...>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(Handle<_receiver_t> Receiver, VOID (_receiver_t::*Procedure)(_sender_t*, _args_t...))
		{
		_handler_t* handler=new Details::EventHandlerWithSender<_sender_t, _receiver_t, _args_t...>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
};


//=========================
// Event without Arguments
//=========================

template <class _sender_t>
class Event<_sender_t>: public Details::EventBaseTyped<_sender_t>
{
public:
	// Using
	using _handler_t=Details::EventHandler<_sender_t>;

	// Con-/Destructors
	Event() {}
	Event(Event const&)=delete;
	Event(Event&&)=delete;

	// Access
	inline operator BOOL() { return this->pFirst!=nullptr; }
	VOID operator()(_sender_t* Sender) { this->RunInternal(Sender); }

	// Modification
	inline VOID Add(VOID (*Procedure)())
		{
		_handler_t* handler=new Details::EventHandlerProcedure<_sender_t>(Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(_receiver_t* Receiver, VOID (_receiver_t::*Procedure)())
		{
		_handler_t* handler=new Details::EventHandlerWithReceiver<_sender_t, _receiver_t>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(Handle<_receiver_t> Receiver, VOID (_receiver_t::*Procedure)())
		{
		_handler_t* handler=new Details::EventHandlerWithReceiver<_sender_t, _receiver_t>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(_receiver_t* Receiver, VOID (_receiver_t::*Procedure)(_sender_t*))
		{
		_handler_t* handler=new Details::EventHandlerWithSender<_sender_t, _receiver_t>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
	template <class _receiver_t> VOID Add(Handle<_receiver_t> Receiver, VOID (_receiver_t::*Procedure)(_sender_t*))
		{
		_handler_t* handler=new Details::EventHandlerWithSender<_sender_t, _receiver_t>(Receiver, Procedure);
		ListAppend(&this->pFirst, handler);
		}
};
