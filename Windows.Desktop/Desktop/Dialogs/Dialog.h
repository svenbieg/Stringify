//==========
// Dialog.h
//==========

#pragma once


//=======
// Using
//=======

#include "UI/Overlapped.h"


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Dialog
//========

class Dialog: public UI::Overlapped
{
public:
	// Common
	INT Show();

protected:
	// Con-/Destructors
	Dialog();

	// Common
	LRESULT HandleMessage(UINT Message, WPARAM WParam, LPARAM LParam, BOOL& Handled)override;

private:
	// Common
	VOID Center();
	Status m_Status;
};

}}