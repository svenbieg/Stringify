//=========
// Popup.h
//=========

#pragma once


//=======
// Using
//=======

#include "UI/Overlapped.h"
#include "DynamicHandle.h"


//===========
// Namespace
//===========

namespace UI {


//=======
// Popup
//=======

class Popup: public Overlapped
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<Popup> Create(Overlapped* Parent=nullptr)
		{
		return Object::Create<Popup>(Parent);
		}

	// Common
	DynamicHandle<Popup, Window> Content;

protected:
	// Con-/Destructors
	Popup(Overlapped* Parent);

	// Common
	UINT Release()noexcept override;

private:
	// Common
	VOID OnContentChanged(Handle<Window> Content);
};

}