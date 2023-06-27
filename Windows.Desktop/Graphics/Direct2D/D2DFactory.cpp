//================
// D2DFactory.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#pragma comment(lib, "d2d1.lib")

#include "D2DFactory.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Common
//========

VOID D2DFactory::CreateRenderTarget(ID2D1DCRenderTarget** pptarget)
{
auto props=D2D1::RenderTargetProperties(
	D2D1_RENDER_TARGET_TYPE_DEFAULT,
	D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
	0, 0, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT);
pFactory->CreateDCRenderTarget(&props, pptarget);
}

D2DFactory* D2DFactory::Open()
{
if(!hCurrent)
	hCurrent=new D2DFactory();
return hCurrent;
}


//==========================
// Con-/Destructors Private
//==========================

D2DFactory::D2DFactory()
{
D2D1_FACTORY_OPTIONS d2dfo;
ZeroMemory(&d2dfo, sizeof(D2D1_FACTORY_OPTIONS));
#ifdef _DEBUG
d2dfo.debugLevel=D2D1_DEBUG_LEVEL_INFORMATION;
#endif
D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &d2dfo, pFactory.GetAddressOf());
}


//================
// Common Private
//================

Handle<D2DFactory> D2DFactory::hCurrent;

}}