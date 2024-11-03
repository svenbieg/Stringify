//===========
// ToolTip.h
//===========

#pragma once


//=======
// Using
//=======

#include "Devices/Timers/Timer.h"
#include "UI/Controls/Interactive.h"
#include "UI/Controls/Panel.h"
#include "Sentence.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {
		namespace Menus {


//==========
// Tool-Tip
//==========

class ToolTip: public Object
{
public:
	// Using
	using Timer=Devices::Timers::Timer;

	// Con-/Destructors
	ToolTip(Interactive* Control, Handle<Sentence> Text=nullptr);
	~ToolTip();

	// Common
	VOID Close();
	DynamicHandle<ToolTip, Sentence> Text;

private:
	// Common
	VOID DoClose();
	VOID OnControlClicked();
	VOID OnControlPointerEntered();
	VOID OnControlPointerLeft();
	VOID OnTextChanged();
	VOID OnTimerTriggered();
	Interactive* m_Control;
	Handle<Panel> m_Panel;
	Handle<Timer> m_Timer;
};

}}}
