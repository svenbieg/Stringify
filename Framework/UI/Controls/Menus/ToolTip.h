//===========
// ToolTip.h
//===========

#pragma once


//=======
// Using
//=======

#include "Core/Timer.h"
#include "UI/Controls/Interactive.h"
#include "UI/Controls/Panel.h"


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
	// Using
	using Timer=Core::Timer;

	// Common
	VOID DoClose();
	VOID OnControlClicked();
	VOID OnControlPointerEntered();
	VOID OnControlPointerLeft();
	VOID OnTextChanged();
	VOID OnTimerTriggered();
	Handle<Panel> hPanel;
	Handle<Timer> hTimer;
	Interactive* pControl;
};

}}}