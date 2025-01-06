//===========
// ToolTip.h
//===========

#pragma once


//=======
// Using
//=======

#include "Culture/Sentence.h"
#include "Timing/Timer.h"
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
	// Using
	using Sentence=Culture::Sentence;
	using Timer=Timing::Timer;

	// Con-/Destructors
	~ToolTip();
	static inline Handle<ToolTip> Create(Interactive* Parent, Handle<Sentence> Text=nullptr)
		{
		return new ToolTip(Parent, Text);
		}

	// Common
	VOID Close();
	DynamicHandle<ToolTip, Sentence> Text;

private:
	// Con-/Destructors
	ToolTip(Interactive* Parent, Handle<Sentence> Text);

	// Common
	VOID DoClose();
	VOID OnParentClicked();
	VOID OnParentPointerEntered();
	VOID OnParentPointerLeft();
	VOID OnTextChanged();
	VOID OnTimerTriggered();
	Handle<Panel> m_Panel;
	Interactive* m_Parent;
	Handle<Timer> m_Timer;
};

}}}