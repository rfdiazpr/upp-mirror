#include "OSVGS.h"

#ifdef _WIN32

void	OSVGSDirect2D::SetFont(const Font &font){
	if(current_font==font) return;
	
	current_font=font;
	if(m_pTextFormat) SafeRelease(&m_pTextFormat);
	
	HRESULT hr = m_pDWriteFactory->CreateTextFormat(
	        WString(font.GetFaceName()), // Font family name.
	        NULL,                       // Font collection (NULL sets it to use the system font collection).
	        font.IsBold()?DWRITE_FONT_WEIGHT_BOLD:DWRITE_FONT_WEIGHT_REGULAR,
	        font.IsItalic()?DWRITE_FONT_STYLE_ITALIC:DWRITE_FONT_STYLE_NORMAL,
	        DWRITE_FONT_STRETCH_NORMAL,
	        (float)font.GetHeight(),
	        L"", // en-us
	        &m_pTextFormat
	        );
}

OSVGSDirect2D::OSVGSDirect2D(const Size &sz_,int flags_,HDC hdc): OSVGS(sz_,flags_){
	m_pD2DFactory=0;
	m_pDCRT=0;
	m_pBrush=0;
	m_pDWriteFactory=0;
	m_pTextFormat=0;
	current_font=Null;
			
    HRESULT hr=D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM,
                D2D1_ALPHA_MODE_IGNORE),
            0,
            0,
            D2D1_RENDER_TARGET_USAGE_NONE,
            D2D1_FEATURE_LEVEL_DEFAULT
            );

    m_pD2DFactory->CreateDCRenderTarget(&props, &m_pDCRT);
    
    if(!flags&OSVGS_ANTIALIASED) m_pDCRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	
	current_color.r=4;
	SetColor(Black()); // Create initial brush
	
	RECT rc=Rect(0,0,(int)sz.cx,(int)sz.cy);
	m_pDCRT->BindDC(hdc, &rc);
    m_pDCRT->BeginDraw();
	m_pDCRT->SetTransform(D2D1::Matrix3x2F::Identity());
	
	if (SUCCEEDED(hr)){
    	hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(m_pDWriteFactory),
            reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
            );
	}
	
	for(int i=0;i<5;i++) m_pStrokeStyle[i]=0;

	float dashes[5][6]={
		{1,0},	// Solid in fact
		{5,3},
		{1,2},
		{8,2,1,2},
		{8,2,1,2,1,2}
	};

	int dashcounts[5]={2,2,2,4,6};
	
	for(int i=0;i<5;i++) if (SUCCEEDED(hr)){
        hr = m_pD2DFactory->CreateStrokeStyle(
            D2D1::StrokeStyleProperties(
                D2D1_CAP_STYLE_FLAT,
                D2D1_CAP_STYLE_FLAT,
                D2D1_CAP_STYLE_FLAT,
                D2D1_LINE_JOIN_MITER,
                1.0f,
                D2D1_DASH_STYLE_CUSTOM,
                0.0f),
            dashes[i],
            dashcounts[i],
            &m_pStrokeStyle[i]
            );
	}
	
	translation.Add(D2D1::Matrix3x2F::Translation(0, 0));
}

OSVGSDirect2D::~OSVGSDirect2D(){
	m_pDCRT->EndDraw();
	
	for(int i=0;i<5;i++) SafeRelease(&m_pStrokeStyle[i]);
	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pBrush);
    SafeRelease(&m_pDCRT);
    SafeRelease(&m_pD2DFactory);
    
}
void	OSVGSDirect2D::Flush(){ m_pDCRT->Flush(); }

Sizef	OSVGSDirect2D::GetDPI(){ 
	float cx=96,cy=96;
	m_pDCRT->GetDpi(&cx,&cy);
	return Sizef(cx,cy);
}

Sizef	OSVGSDirect2D::GetSize(){ return sz; }

void	OSVGSDirect2D::BeginClip(const Rectf &rect, bool offset){
	m_pDCRT->PushAxisAlignedClip(D2D1::RectF((float)rect.left,(float)rect.top,(float)rect.right,(float)rect.bottom),D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	if(offset) translation.Add(D2D1::Matrix3x2F::Translation((float)rect.left, (float)rect.top)*translation[translation.GetCount()-1]);
	else translation.Add(translation[translation.GetCount()-1]);
	m_pDCRT->SetTransform(translation[translation.GetCount()-1]);
}

void	OSVGSDirect2D::EndClip(){
	if(translation.GetCount()) translation.Remove(translation.GetCount()-1);
	m_pDCRT->SetTransform(translation[translation.GetCount()-1]);
	m_pDCRT->PopAxisAlignedClip();
}

void	OSVGSDirect2D::Clear(const RGBA &color){ m_pDCRT->Clear(D2D1::ColorF(color.r/255.0f,color.g/255.0f,color.b/255.0f,color.a/255.0f)); }

void	OSVGSDirect2D::DrawLine(const Pointf &p0,const Pointf &p1, double width, const RGBA &color, int style){
	SetColor(color);
	m_pDCRT->DrawLine(D2D1::Point2F((float)p0.x,(float)p0.y),D2D1::Point2F((float)p1.x,(float)p1.y),m_pBrush,(float)width,style?m_pStrokeStyle[style]:0);
}

void	OSVGSDirect2D::DrawPolyline(const Vector<Pointf> &vertices, double width, const RGBA &color, int style){
	if(!vertices.GetCount()) return;
	SetColor(color);
	
	ID2D1PathGeometry* pg = 0;
	ID2D1GeometrySink* pgs = 0;
	
	m_pD2DFactory->CreatePathGeometry(&pg);
	
	pg->Open(&pgs);
	pgs->BeginFigure(D2D1::Point2F((float)vertices[0].x,(float)vertices[0].y),D2D1_FIGURE_BEGIN_HOLLOW);
	for(int i=1;i<vertices.GetCount();i++) pgs->AddLine(D2D1::Point2F((float)vertices[i].x,(float)vertices[i].y));
	pgs->EndFigure(D2D1_FIGURE_END_OPEN); // pgs->EndFigure(D2D1_FIGURE_END_CLOSED);
	if(style) m_pDCRT->DrawGeometry(pg,m_pBrush,(float)width,m_pStrokeStyle[style]);
	else m_pDCRT->DrawGeometry(pg,m_pBrush,(float)width);
	pgs->Close();
	SafeRelease(&pgs);
	SafeRelease(&pg);
}

void	OSVGSDirect2D::FillPolygon(const Vector<Pointf> &vertices, const RGBA &color){
	if(!vertices.GetCount()) return;
	SetColor(color);

	ID2D1PathGeometry* pg = 0;
	ID2D1GeometrySink* pgs = 0;

	m_pD2DFactory->CreatePathGeometry(&pg);

	pg->Open(&pgs);
	pgs->BeginFigure(D2D1::Point2F((float)vertices[0].x,(float)vertices[0].y),D2D1_FIGURE_BEGIN_FILLED);
	for(int i=1;i<vertices.GetCount();i++) pgs->AddLine(D2D1::Point2F((float)vertices[i].x,(float)vertices[i].y));
	pgs->EndFigure(D2D1_FIGURE_END_CLOSED);
	m_pDCRT->FillGeometry(pg,m_pBrush);
	pgs->Close();
	SafeRelease(&pgs);
}

void	OSVGSDirect2D::DrawImage(const Pointf &p,const Image &img){
	ID2D1Bitmap *bitmap=0;
	D2D1_BITMAP_PROPERTIES prop;
	prop.pixelFormat=D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED); //D2D1_ALPHA_MODE_IGNORE);
	prop.dpiX=(float)GetDPI().cx;
	prop.dpiY=(float)GetDPI().cy;
	
	m_pDCRT->CreateBitmap(D2D1::SizeU(img.GetSize().cx,img.GetSize().cy), //D2D1_SIZE_U
		(RGBA*)~img, //  [in, optional]  void *srcData,
		(UINT32)img.GetWidth()*sizeof(RGBA), // UINT32 pitch,
		&prop, // [in]            const D2D1_BITMAP_PROPERTIES *bitmapProperties,
		&bitmap); // [out]           ID2D1Bitmap **bitmap

	D2D1_RECT_F rect = D2D1::RectF((float)p.x,(float)p.y,(float)(p.x+img.GetWidth()),(float)(p.y+img.GetHeight()));
	m_pDCRT->DrawBitmap(bitmap,rect,1.0f);
	SafeRelease(&bitmap);
}

void	OSVGSDirect2D::FillRectangle(const Rectf &rect, const RGBA &color){
	SetColor(color);
	m_pDCRT->FillRectangle(D2D1::RectF((float)rect.left,(float)rect.top,(float)rect.right,(float)rect.bottom),m_pBrush);
}

void	OSVGSDirect2D::DrawText(const Pointf &p,const String &text,double angle,const Font &font, const RGBA &color){
	SetColor(color);
	SetFont(font);
	
	if(angle!=0.0){
		m_pDCRT->SetTransform(D2D1::Matrix3x2F::Rotation((float)-angle,D2D1::Point2F((float)p.x,(float)p.y))* translation[translation.GetCount()-1]);
		m_pDCRT->DrawText(WString(text), text.GetCharCount(), m_pTextFormat, D2D1::RectF((float)p.x,(float)p.y,1e9f,1e9f), m_pBrush);
		m_pDCRT->SetTransform(translation[translation.GetCount()-1]);
	}
	else m_pDCRT->DrawText(WString(text), text.GetCharCount(), m_pTextFormat, D2D1::RectF((float)p.x,(float)p.y,1e9f,1e9f), m_pBrush);
}

void	OSVGSDirect2D::DrawArc(const Pointf &center, double rx, double ry, double startangle, double stopangle, double width, const RGBA &color, int style){
	SetColor(color);
	
	Pointf p0=ellipse(center,rx,ry,startangle*(M_PI/180.0));
	Pointf p1=ellipse(center,rx,ry,stopangle*(M_PI/180.0));
	
	ID2D1PathGeometry* pg = 0;
	ID2D1GeometrySink* pgs = 0;
	
	m_pD2DFactory->CreatePathGeometry(&pg);
	pg->Open(&pgs);
	
	pgs->BeginFigure(D2D1::Point2F((float)p0.x,(float)p0.y),D2D1_FIGURE_BEGIN_HOLLOW);
	pgs->AddArc(D2D1::ArcSegment(D2D1::Point2F((float)p1.x,(float)p1.y+0.0001f),D2D1::SizeF((float)rx,(float)ry),0,D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,fabs(stopangle-startangle)<180.0?D2D1_ARC_SIZE_SMALL:D2D1_ARC_SIZE_LARGE));
	pgs->EndFigure(D2D1_FIGURE_END_OPEN);

	if(style) m_pDCRT->DrawGeometry(pg,m_pBrush,(float)width,m_pStrokeStyle[style]);
	else m_pDCRT->DrawGeometry(pg,m_pBrush,(float)width);
	pgs->Close();
	SafeRelease(&pgs);
	SafeRelease(&pg);
}

void	OSVGSDirect2D::FillEllipse(const Pointf &center, double rx, double ry, const RGBA &color){
	SetColor(color);
	m_pDCRT->FillEllipse(D2D1::Ellipse(D2D1::Point2F((float)center.x,(float)center.y),(float)rx,(float)ry),m_pBrush);
}

#endif

