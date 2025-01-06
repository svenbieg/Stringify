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
	static inline Handle<ProgressBar> Create(Window* Parent) { return new ProgressBar(Parent); }

	// Common
	Handle<Brush> GetBorderBrush()override;

private:
	// Con-/Destructors
	ProgressBar(Window* Parent);
};

}}