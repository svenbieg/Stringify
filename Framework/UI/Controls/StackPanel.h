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
	StackPanel(Orientation Orientation=Orientation::Horizontal);
	StackPanel(Window* Parent, Orientation Orientation=Orientation::Horizontal);

	// Common
	Alignment AlignChildren;
	virtual SIZE GetMinSize(RenderTarget* Target)override;
	VOID Rearrange(RenderTarget* Target, RECT& Rect)override;

private:
	// Common
	Orientation m_Orientation;
};

}}