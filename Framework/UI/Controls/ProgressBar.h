//===============
// ProgressBar.h
//===============

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Panel.h"


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
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<ProgressBar> Create(Window* Parent) { return Object::Create<ProgressBar>(Parent); }

	// Common
	Handle<Brush> GetBorderBrush()override;

private:
	// Con-/Destructors
	ProgressBar(Window* Parent);
};

}}