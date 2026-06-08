//===================
// CriticalSection.h
//===================

#pragma once


//=======
// Using
//=======

#include "Mutex.h"


//===========
// Namespace
//===========

namespace Concurrency {


//==================
// Critical Section
//==================

class CriticalSection: public Mutex
{
public:
	// Con-/Destructors
	CriticalSection()=default;
	CriticalSection(CriticalSection const&)=delete;
};

}