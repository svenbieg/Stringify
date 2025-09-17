//==================
// RenderTarget.cpp
//==================

#include "pch.h"


//=======
// Using
//=======

#include "Graphics/RenderTarget.h"


//===========
// Namespace
//===========

namespace Graphics {


//==================
// Con-/Destructors
//==================

RenderTarget::~RenderTarget()
{
for(auto it=m_Bitmaps.cbegin(); it.has_current(); it.move_next())
	{
	auto bmp=it.get_key();
	auto d2d_bmp=it.get_value();
	bmp->Changed.Remove(this);
	bmp->Destroyed.Remove(this);
	d2d_bmp->Release();
	}
for(auto it=m_Brushes.cbegin(); it.has_current(); it.move_next())
	{
	auto brush=it.get_key();
	auto d2d_brush=it.get_value();
	brush->Changed.Remove(this);
	brush->Destroyed.Remove(this);
	d2d_brush->Release();
	}
}


//========
// Common
//========

VOID RenderTarget::BeginDraw(HDC hdc, RECT const& rc)
{
::RECT rcc({ 0, 0, rc.Right, rc.Bottom });
m_Target->BindDC(hdc, &rcc);
m_Target->BeginDraw();
}

VOID RenderTarget::Clip(POINT const& offset, RECT const& clip)
{
POINT pt_clip(clip.Left, clip.Top);
m_Offset=pt_clip-offset;
auto d2d_clip=D2D1::RectF(clip.Left, clip.Top, clip.Right, clip.Bottom);
m_Target->PushAxisAlignedClip(d2d_clip, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
auto translate=D2D1::Matrix3x2F::Translation(offset.Left, offset.Top);
m_Target->SetTransform(translate);
}

VOID RenderTarget::DrawBitmap(RECT const& dst, Bitmap* bmp, RECT const& src)
{
auto d2d_bmp=GetBitmap(bmp);
auto d2d_dst=D2D1::RectF(dst.Left, dst.Top, dst.Right, dst.Bottom);
auto d2d_src=D2D1::RectF(src.Left, src.Top, src.Right, src.Bottom);
m_Target->DrawBitmap(d2d_bmp, d2d_dst, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, d2d_src);
}

VOID RenderTarget::DrawLine(POINT const& from, POINT const& to, Brush* brush, UINT width)
{
auto d2d_from=D2DPoint(from);
auto d2d_to=D2DPoint(to);
auto d2d_brush=GetBrush(brush);
m_Target->DrawLine(d2d_from, d2d_to, d2d_brush, width);
}

VOID RenderTarget::DrawPolygon(POINT const* points, UINT count, Brush* brush, UINT width)
{
if(!count)
	return;
auto geometry=m_D2DFactory->CreatePathGeometry();
ID2D1GeometrySink* sink=nullptr;
geometry->Open(&sink);
auto pt=D2D1::Point2F(points[0].Left, points[0].Top);
sink->BeginFigure(pt, D2D1_FIGURE_BEGIN_HOLLOW);
for(UINT u=1; u<count; u++)
	sink->AddLine(D2D1::Point2F(points[u].Left, points[u].Top));
sink->EndFigure(D2D1_FIGURE_END_CLOSED);
sink->Close();
sink->Release();
auto d2d_brush=GetBrush(brush);
m_Target->DrawGeometry(geometry, d2d_brush, width);
}

VOID RenderTarget::DrawRect(RECT const& rc, Brush* brush, UINT width)
{
auto d2d_rc=D2D1::RectF((FLOAT)rc.Left+0.5f, (FLOAT)rc.Top+0.5f, (FLOAT)rc.Right-0.5f, (FLOAT)rc.Bottom-0.5f);
auto d2d_brush=GetBrush(brush);
m_Target->DrawRectangle(d2d_rc, d2d_brush, width);
}

VOID RenderTarget::DrawText(RECT const& rc, FLOAT scale, Font* font, Brush* brush, LPCTSTR text, UINT len)
{
if(len==0)
	len=StringHelper::Length(text);
D2D1_MATRIX_3X2_F mx_transform;
m_Target->GetTransform(&mx_transform);
auto mx_translate=D2D1::Matrix3x2F::Translation(rc.Left, rc.Top);
auto mx_scale=D2D1::Matrix3x2F::Scale(scale, scale);
m_Target->SetTransform(mx_scale*mx_transform*mx_translate);
auto d2d_format=font->GetFormat();
auto d2d_rc=D2D1::RectF(0, 0, rc.GetWidth(), rc.GetHeight());
auto d2d_brush=GetBrush(brush);
#ifdef _UNICODE
LPCWSTR str=text;
#else
if(len>255)
	throw BufferOverrunException();
WCHAR str[256];
StringHelper::Copy(str, 256, text);
#endif
m_Target->DrawText(str, len, d2d_format, d2d_rc, d2d_brush);
m_Target->SetTransform(mx_transform);
}

VOID RenderTarget::EndDraw()
{
m_Target->EndDraw();
}

VOID RenderTarget::FillPolygon(POINT const* points, UINT count, Brush* brush)
{
if(!count)
	return;
auto geometry=m_D2DFactory->CreatePathGeometry();
ID2D1GeometrySink* sink=nullptr;
geometry->Open(&sink);
auto pt=D2D1::Point2F(points[0].Left, points[0].Top);
sink->BeginFigure(pt, D2D1_FIGURE_BEGIN_FILLED);
for(UINT u=1; u<count; u++)
	sink->AddLine(D2D1::Point2F(points[u].Left, points[u].Top));
sink->EndFigure(D2D1_FIGURE_END_CLOSED);
sink->Close();
sink->Release();
auto d2d_brush=GetBrush(brush);
m_Target->FillGeometry(geometry, d2d_brush);
}

VOID RenderTarget::FillRect(RECT const& rc, Brush* brush)
{
auto d2d_rc=D2D1::RectF(rc.Left, rc.Top, rc.Right, rc.Bottom);
auto d2d_brush=GetBrush(brush);
m_Target->FillRectangle(d2d_rc, d2d_brush);
}

SIZE RenderTarget::MeasureText(Font* font, FLOAT scale, LPCTSTR text, UINT len)
{
if(!len)
	len=StringHelper::Length(text);
#ifdef _UNICODE
LPCWSTR str=text;
#else
WCHAR str[128];
StringHelper::Copy(str, 128, text, len);
#endif
if(!m_DWriteFactory)
	m_DWriteFactory=DWriteFactory::Get();
auto d2d_format=font->GetFormat();
auto layout=m_DWriteFactory->CreateTextLayout(str, len, d2d_format);
DWRITE_TEXT_METRICS metrics={ 0 };
layout->GetMetrics(&metrics);
return SIZE (metrics.widthIncludingTrailingWhitespace*scale+1, metrics.height*scale+1);
}

VOID RenderTarget::Unclip()
{
m_Target->SetTransform(D2D1::Matrix3x2F::Identity());
m_Target->PopAxisAlignedClip();
}


//==========================
// Con-/Destructors Private
//==========================

RenderTarget::RenderTarget()
{
m_D2DFactory=D2DFactory::Get();
m_Target=m_D2DFactory->CreateRenderTarget();
m_Target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
m_Target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
}


//================
// Common Private
//================

D2D1_POINT_2F RenderTarget::D2DPoint(POINT const& pt)
{
auto d2d_pt=D2D1::Point2F(pt.Left, pt.Top);
if(pt.Left==0)
	d2d_pt.x+=0.5f;
if(pt.Top==0)
	d2d_pt.y+=0.5f;
return d2d_pt;
}

ID2D1Bitmap* RenderTarget::GetBitmap(Bitmap* bmp)
{
ID2D1Bitmap* d2d_bmp=nullptr;
if(m_Bitmaps.try_get(bmp, &d2d_bmp))
	return d2d_bmp;
D2D1_BITMAP_PROPERTIES props;
MemoryHelper::Zero(&props, sizeof(props));
props.pixelFormat=D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
auto buf=bmp->Begin();
auto size=bmp->GetDimensions();
auto d2d_size=D2D1::SizeU(size.Width, size.Height);
m_Target->CreateBitmap(d2d_size, buf, size.Width*4, props, &d2d_bmp);
if(!d2d_bmp)
	throw InvalidArgumentException();
m_Bitmaps.add(bmp, d2d_bmp);
bmp->Changed.Add(this, &RenderTarget::OnBitmapChanged);
bmp->Destroyed.Add(this, &RenderTarget::OnBitmapChanged);
return d2d_bmp;
}

ID2D1SolidColorBrush* RenderTarget::GetBrush(Brush* brush)
{
ID2D1SolidColorBrush* d2d_brush=nullptr;
if(m_Brushes.try_get(brush, &d2d_brush))
	return d2d_brush;
COLOR c=brush->GetColor();
D2D1_COLOR_F color({ (FLOAT)c.GetRed()/255, (FLOAT)c.GetGreen()/255, (FLOAT)c.GetBlue()/255, (FLOAT)c.GetAlpha()/255 });
m_Target->CreateSolidColorBrush(color, &d2d_brush);
if(!d2d_brush)
	throw InvalidArgumentException();
m_Brushes.add(brush, d2d_brush);
brush->Changed.Add(this, &RenderTarget::OnBrushChanged);
brush->Destroyed.Add(this, &RenderTarget::OnBrushChanged);
return d2d_brush;
}

VOID RenderTarget::OnBitmapChanged(Bitmap* bmp)
{
ID2D1Bitmap* d2d_bmp=nullptr;
m_Bitmaps.remove(bmp, &d2d_bmp);
bmp->Changed.Remove(this);
bmp->Destroyed.Remove(this);
d2d_bmp->Release();
}

VOID RenderTarget::OnBrushChanged(Brush* brush)
{
ID2D1SolidColorBrush* d2d_brush=nullptr;
m_Brushes.remove(brush, &d2d_brush);
brush->Changed.Remove(this);
brush->Destroyed.Remove(this);
d2d_brush->Release();
}

}