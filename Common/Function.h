//============
// Function.h
//============

#pragma once


//==========
// Function
//==========

template <class _ret_t, class... _args_t>
class Function: public Object
{
public:
	// Common
	virtual _ret_t Run(_args_t... Arguments)=0;

protected:
	// Con-/Destructors
	Function()=default;
};


//=================
// Member-Function
//=================

template <class _owner_t, class _ret_t, class... _args_t>
class MemberFunction: public Function<_ret_t, _args_t...>
{
public:
	// Types
	typedef _ret_t (_owner_t::*_func_t)(_args_t...);

	// Con-/Destructors
	MemberFunction(_owner_t* Owner, _func_t Function):
		pFunction(Function),
		pOwner(Owner)
		{}

	// Common
	inline _ret_t Run(_args_t... Arguments)override
		{
		return (pOwner->*pFunction)(Arguments...);
		}

private:
	// Common
	_func_t pFunction;
	_owner_t* pOwner;
};
