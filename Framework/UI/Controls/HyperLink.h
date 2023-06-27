//=============
// HyperLink.h
//=============

#pragma once


//=======
// Using
//=======

#include "Graphics/Font.h"
#include "Interactive.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//============
// Hyper-Link
//============

class HyperLink: public Interactive
{
public:
	// Con-/Destructors
	HyperLink(Window* Parent=nullptr);

	// Common
	SIZE GetMinSize(RenderTarget* Target)override;
	Handle<String> Link;
	VOID Render(RenderTarget* Target, RECT& Rect)override;
	Handle<String> Text;
};

}}