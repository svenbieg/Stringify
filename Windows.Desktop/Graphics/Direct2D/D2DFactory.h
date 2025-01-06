//==============
// D2DFactory.h
//==============

#pragma once


//=======
// Using
//=======

#include <d2d1_2.h>
#include "ComPointer.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//=============
// D2D-Factory
//=============

class D2DFactory: public Object
{
public:
	// Con-/Destructors
	~D2DFactory();
	static Handle<D2DFactory> Get();

	// Common
	ComPointer<ID2D1PathGeometry> CreatePathGeometry();
	ComPointer<ID2D1DCRenderTarget> CreateRenderTarget();
	ID2D1Factory* GetFactory()const { return m_Factory; }

private:
	// Con-/Destructors
	D2DFactory();

	// Common
	ComPointer<ID2D1Factory> m_Factory;
	static D2DFactory* s_Current;
};

}}