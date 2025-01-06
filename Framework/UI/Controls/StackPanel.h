//==============
// StackPanel.h
//==============

#pragma once


//=======
// Using
//=======

#include "UI/Alignment.h"
#include "UI/Orientation.h"
#include "Panel.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//=============
// Stack-Panel
//=============

class StackPanel: public Panel
{
public:
	// Con-/Destructors
	static inline Handle<StackPanel> Create(Window* Parent, Orientation Orientation=Orientation::Horizontal)
		{
		return new StackPanel(Parent, Orientation);
		}

	// Common
	Alignment AlignChildren;
	virtual SIZE GetMinSize(RenderTarget* Target)override;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;

protected:
	// Con-/Destructors
	StackPanel(Window* Parent, Orientation Orientation=Orientation::Horizontal);

private:
	// Common
	Orientation m_Orientation;
};

}}