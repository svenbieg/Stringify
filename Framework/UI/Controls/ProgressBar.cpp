//=================
// ProgressBar.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "ProgressBar.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//========
// Common
//========

Handle<Brush> ProgressBar::GetBorderBrush()
{
auto theme=GetTheme();
return theme->BorderBrush;
}


//==========================
// Con-/Destructors Private
//==========================

ProgressBar::ProgressBar(Window* parent):
Panel(parent)
{
MinSize.Set(0, 14);
}

}}