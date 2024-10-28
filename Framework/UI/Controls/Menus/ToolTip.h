//===========
// ToolTip.h
//===========

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Interactive.h"
#include "UI/Controls/Panel.h"
#include "UI/Timer.h"
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
	Handle<Panel> hPanel;
	Handle<Timer> m_Timer;
	Interactive* pControl;
};

}}}
