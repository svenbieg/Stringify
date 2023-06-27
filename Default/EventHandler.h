//================
// EventHandler.h
//================

#pragma once


//=======
// Using
//=======

#include "Handle.h"


//===========
// Namespace
//===========

namespace Details {


//===============
// Event-Handler
//===============

template <class _sender_t, class... _args_t>
class EventHandler
{
public:
	// Con-/Destructors
	virtual ~EventHandler() {}

	// Common
	inline EventHandler* GetNext()const { return pNext; }
	inline EventHandler* GetPrevious()const { return pPrevious; }
	virtual VOID* GetReceiver()const=0;
	inline VOID Invalidate() { uRunning|=0x80000000; }
	inline BOOL IsRunning()const { return (uRunning&0x7FFFFFFF)>0; }
	inline BOOL IsValid()const { return (uRunning!=0x80000000); }
	virtual VOID Run(_sender_t* Sender, _args_t... Arguments)=0;
	inline VOID SetNext(EventHandler* Next) { pNext=Next; }
	inline VOID SetPrevious(EventHandler* Previous) { pPrevious=Previous; }

protected:
	// Con-/Destructors
	EventHandler():
		pNext(nullptr),
		pPrevious(nullptr),
		uRunning(0)
		{}

	// Common
	EventHandler* pNext;
	EventHandler* pPrevious;
	UINT uRunning;
};


//=========================
// Event-Handler Procedure
//=========================

template <class _sender_t, class... _args_t>
class EventHandlerProcedure: public EventHandler<_sender_t, _args_t...>
{
public:
	// Definitions
	typedef VOID (*_proc_t)();

	// Con-/Destructors
	EventHandlerProcedure(_proc_t Procedure): pProcedure(Procedure) {}

	// Common
	inline VOID* GetReceiver()const override { return pProcedure; }
	VOID Run(_sender_t* Sender, _args_t... Arguments)override
		{
		if(this->uRunning&0x80000000)
			return;
		this->uRunning++;
		(*pProcedure)();
		this->uRunning--;
		}

private:
	// Common
	_proc_t pProcedure;
};


//=============================
// Event-Handler with Receiver
//=============================

template <class _sender_t, class _receiver_t, class... _args_t>
class EventHandlerWithReceiver: public EventHandler<_sender_t, _args_t...>
{
public:
	// Definitions
	typedef VOID (_receiver_t::*_proc_t)();

	// Con-/Destructors
	EventHandlerWithReceiver(_receiver_t* Receiver, _proc_t Procedure): pProcedure(Procedure), pReceiver(Receiver) {}

	// Common
	inline VOID* GetReceiver()const override { return pReceiver; }
	VOID Run(_sender_t* Sender, _args_t... Arguments)override
		{
		if(this->uRunning&0x80000000)
			return;
		this->uRunning++;
		(pReceiver->*pProcedure)();
		this->uRunning--;
		}

private:
	// Common
	_proc_t pProcedure;
	_receiver_t* pReceiver;
};


//==============================
// Event-Handler with Arguments
//==============================

template <class _sender_t, class _receiver_t, class... _args_t>
class EventHandlerWithArgs: public EventHandler<_sender_t, _args_t...>
{
public:
	// Definitions
	typedef VOID (_receiver_t::*_proc_t)(_args_t...);

	// Con-/Destructors
	EventHandlerWithArgs(_receiver_t* Receiver, _proc_t Procedure): pProcedure(Procedure), pReceiver(Receiver) {}

	// Common
	inline VOID* GetReceiver()const override { return pReceiver; }
	VOID Run(_sender_t* Sender, _args_t... Arguments)override
		{
		if(this->uRunning&0x80000000)
			return;
		this->uRunning++;
		(pReceiver->*pProcedure)(Arguments...);
		this->uRunning--;
		}

private:
	// Common
	_proc_t pProcedure;
	_receiver_t* pReceiver;
};


//===========================
// Event-Handler with Sender
//===========================

template <class _sender_t, class _receiver_t, class... _args_t>
class EventHandlerWithSender: public EventHandler<_sender_t, _args_t...>
{
public:
	// Definitions
	typedef VOID (_receiver_t::*_proc_t)(_sender_t*, _args_t...);

	// Con-/Destructors
	EventHandlerWithSender(_receiver_t* Receiver, _proc_t Procedure): pProcedure(Procedure), pReceiver(Receiver) {}

	// Common
	inline VOID* GetReceiver()const override { return pReceiver; }
	VOID Run(_sender_t* Sender, _args_t... Arguments)override
		{
		if(this->uRunning&0x80000000)
			return;
		this->uRunning++;
		(pReceiver->*pProcedure)(Sender, Arguments...);
		this->uRunning--;
		}

private:
	// Common
	_proc_t pProcedure;
	_receiver_t* pReceiver;
};

}