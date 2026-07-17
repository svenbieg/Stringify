//================
// D2DFactory.cpp
//================

#include "D2DFactory.h"


//=======
// Using
//=======

#pragma comment(lib, "d2d1.lib")

#include "ErrorHelper.h"
#include "MemoryHelper.h"


//===========
// Namespace
//===========

namespace Graphics {
	namespace Direct2D {


//========
// Common
//========

ComPointer<ID2D1PathGeometry> D2DFactory::CreatePathGeometry()
{
ComPointer<ID2D1PathGeometry> geometry;
HRESULT hr=m_Factory->CreatePathGeometry(geometry.AddressOf());
ErrorHelper::ThrowIfFailed(hr);
return geometry;
}

ComPointer<ID2D1DCRenderTarget> D2DFactory::CreateRenderTarget()
{
auto props=D2D1::RenderTargetProperties(
	D2D1_RENDER_TARGET_TYPE_DEFAULT,
	D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
	0, 0, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT);
ComPointer<ID2D1DCRenderTarget> target;
HRESULT hr=m_Factory->CreateDCRenderTarget(&props, target.AddressOf());
ErrorHelper::ThrowIfFailed(hr);
return target;
}


//==========================
// Con-/Destructors Private
//==========================

D2DFactory::D2DFactory()
{
HRESULT hr=CoInitialize(nullptr);
ErrorHelper::ThrowIfFailed(hr);
D2D1_FACTORY_OPTIONS d2dfo;
MemoryHelper::Fill(&d2dfo, sizeof(d2dfo), 0);
#ifdef _DEBUG
d2dfo.debugLevel=D2D1_DEBUG_LEVEL_INFORMATION;
#endif
hr=D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &d2dfo, (VOID**)m_Factory.AddressOf());
ErrorHelper::ThrowIfFailed(hr);
}

}}