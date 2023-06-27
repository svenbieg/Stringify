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
	VOID CreateRenderTarget(ID2D1DCRenderTarget** Target);
	ID2D1Factory* Get()const { return pFactory.Get(); }
	static D2DFactory* Open();

private:
	// Con-/Destructors
	D2DFactory();

	// Common
	static Handle<D2DFactory> hCurrent;
	ComPointer<ID2D1Factory> pFactory;
};

}}