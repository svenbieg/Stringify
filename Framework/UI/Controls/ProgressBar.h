//===============
// ProgressBar.h
//===============

#pragma once


//=======
// Using
//=======

#include "Panel.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//==============
// Progress-Bar
//==============

class ProgressBar: public Panel
{
public:
	// Con-/Destructors
	ProgressBar(Window* Parent=nullptr);

	// Common
	Handle<Brush> GetBorderBrush()override;
};

}}