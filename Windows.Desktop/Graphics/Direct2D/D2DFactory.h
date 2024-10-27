//==============
// D2DFactory.h
//==============

#pragma once


//=======
// Using
//=======

#include <d2d1_2.h>


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
	// Common
	ComPointer<ID2D1PathGeometry> CreatePathGeometry();
	ComPointer<ID2D1DCRenderTarget> CreateRenderTarget();
	ID2D1Factory* Get()const { return m_Factory; }
	static Handle<D2DFactory> Open();

private:
	// Con-/Destructors
	D2DFactory();

	// Common
	static Handle<D2DFactory> m_Current;
	ComPointer<ID2D1Factory> m_Factory;
};

}}