//=========
// Panel.h
//=========

#pragma once


//=======
// Using
//=======

#include "UI/Controls/Control.h"


//===========
// Namespace
//===========

namespace UI {
	namespace Controls {


//=======
// Panel
//=======

class Panel: public Control
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<Panel> Create(Window* Parent) { return Object::Create<Panel>(Parent); }

	// Common
	Property<Panel, BOOL> Border;
	virtual Handle<Brush> GetBorderBrush();
	virtual SIZE GetMinSize(RenderTarget* Target)override;
	RECT Padding;
	virtual VOID Rearrange(RenderTarget* Target, RECT& Rect)override;
	virtual VOID Render(RenderTarget* Target, RECT& Rect)override;

protected:
	// Con-/Destructors
	Panel(Window* Parent);

private:
	// Common
	VOID OnBorderChanged();
};

}}