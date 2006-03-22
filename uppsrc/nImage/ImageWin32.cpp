#include "Image.h"

bool ImageFallBack
= true
;

class BitmapInfo32__ {
	Buffer<byte> data;

public:
	operator BITMAPINFO *()        { return (BITMAPINFO *)~data; }
	operator BITMAPINFOHEADER *()  { return (BITMAPINFOHEADER *)~data; }
	BITMAPINFOHEADER *operator->() { return (BITMAPINFOHEADER *)~data; }

	BitmapInfo32__(int cx, int cy);
};

BitmapInfo32__::BitmapInfo32__(int cx, int cy)
:	data(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256)
{
	BITMAPINFOHEADER *hi = (BITMAPINFOHEADER *) ~data;;
	memset(hi, 0, sizeof(BITMAPINFOHEADER));
	hi->biSize = sizeof(BITMAPINFOHEADER);
	hi->biPlanes = 1;
	hi->biBitCount = 32;
	hi->biCompression = BI_RGB;
	hi->biSizeImage = 0;
	hi->biClrUsed = 0;
	hi->biClrImportant = 0;
	hi->biWidth = cx;
	hi->biHeight = cy;
}

void SetSurface(HDC dc, int x, int y, int cx, int cy, RGBA *pixels)
{
	BitmapInfo32__ bi(cx, cy);
	::SetDIBitsToDevice(dc, x, y, cx, cy, 0, 0, 0, cy, pixels,
	                    bi, DIB_RGB_COLORS);
}

void SetSurface(Draw& w, int x, int y, int cx, int cy, RGBA *pixels)
{
	SetSurface(w.GetHandle(), x, y, cx, cy, pixels);
}

void DrawSurface::Init(Draw& w, int _x, int _y, int cx, int cy)
{
	dc = w.GetHandle();
	size = Size(cx, cy);
	x = _x;
	y = _y;
	dcMem = ::CreateCompatibleDC(dc);
	BitmapInfo32__ bi(cx, cy);
	hbmp = CreateDIBSection(dc, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
	hbmpOld = (HBITMAP) ::SelectObject(dcMem, hbmp);
	::BitBlt(dcMem, 0, 0, cx, cy, dc, x, y, SRCCOPY);
}

DrawSurface::DrawSurface(Draw& w, const Rect& r)
{

	Init(w, r.left, r.top, r.Width(), r.Height());
}

DrawSurface::DrawSurface(Draw& w, int x, int y, int cx, int cy)
{
	Init(w, x, y, cx, cy);
}

DrawSurface::~DrawSurface()
{
	::BitBlt(dc, x, y, size.cx, size.cy, dcMem, 0, 0, SRCCOPY);
	::DeleteObject(::SelectObject(dcMem, hbmpOld));
	::DeleteDC(dcMem);
}

Image::Data::Data(ImageBuffer& b)
:	buffer(b)
{
	hbmp = hmask = himg = NULL;
	paintcount = 0;
	refcount = 1;
}

StaticCriticalSection Image::Data::ResLock;
Link<Image::Data>     Image::Data::ResData[1];
int                   Image::Data::ResCount;

void Image::Data::SysRelease()
{
	if(hbmp) {
		DeleteObject(hbmp);
		ResCount--;
	}
	if(hmask) {
		DeleteObject(hmask);
		ResCount--;
	}
	if(himg) {
		DeleteObject(himg);
		ResCount--;
	}
	himg = hbmp = hmask = NULL;
	Unlink();
}

Image::Data::~Data()
{
	INTERLOCKED_(ResLock) {
		SysRelease();
	}
}

typedef BOOL (WINAPI *tAlphaBlend)(HDC hdcDest, int nXOriginDest, int nYOriginDest,
                                   int nWidthDest, int nHeightDest,
                                   HDC hdcSrc, int nXOriginSrc, int nYOriginSrc,
                                   int nWidthSrc, int nHeightSrc, BLENDFUNCTION blendFunction);

static tAlphaBlend fnAlphaBlend()
{
	static tAlphaBlend pSet;
	static bool inited = false;
	if(!inited) {
		inited = true;
		if(HMODULE hDLL = LoadLibrary("msimg32.dll"))
			pSet = (tAlphaBlend) GetProcAddress(hDLL, "AlphaBlend");
	}
	return pSet;
}

void Image::Data::Paint(Draw& w, int x, int y, byte const_alpha, Color c)
{
	INTERLOCKED_(ResLock) {
		int max = IsWinNT() ? 250 : 100;
		while(ResCount > max) {
			Image::Data *l = ResData->GetPrev();
			l->SysRelease();
			l->Unlink();
		}

		HDC dc = w.GetHandle();

		Size sz = buffer.GetSize();
		int  len = sz.cx * sz.cy;
		if(buffer.GetKind() == IMAGE_EMPTY)
			return;

		if(buffer.GetKind() == IMAGE_OPAQUE && !IsNull(c)) {
			w.DrawRect(x, y, sz.cx, sz.cy, c);
			return;
		}

		if(buffer.GetKind() == IMAGE_OPAQUE && const_alpha == 255 && paintcount == 0) {
			SetSurface(w, x, y, sz.cx, sz.cy, buffer);
			paintcount++;
			return;
		}

		Unlink();
		LinkAfter(ResData);

		bool hasAlphaBlend = fnAlphaBlend();

		if(buffer.GetKind() == IMAGE_OPAQUE && (const_alpha == 255 || hasAlphaBlend)) {
			if(!hbmp) {
				BitmapInfo32__ bi(sz.cx, sz.cy);
				hbmp = ::CreateDIBitmap(dc, bi, CBM_INIT, buffer, bi, DIB_RGB_COLORS);
				ResCount++;
			}
			if(const_alpha == 255) {
				HDC dcMem = ::CreateCompatibleDC(dc);
				::SelectObject(dcMem, hbmp);
				::BitBlt(dc, x, y, sz.cx, sz.cy, dcMem, 0, 0, SRCCOPY);
				::DeleteDC(dcMem);
				return;
			}
			else {
				BLENDFUNCTION bf;
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.SourceConstantAlpha = const_alpha;
				bf.AlphaFormat = 0;
				HDC dcMem = ::CreateCompatibleDC(dc);
				::SelectObject(dcMem, hbmp);
				::fnAlphaBlend()(dc, x, y, sz.cx, sz.cy, dcMem, 0, 0, sz.cx, sz.cy, bf);
				::DeleteDC(dcMem);
				return;
			}
		}

		if(buffer.GetKind() == IMAGE_MASK && const_alpha == 255) {
			HDC dcMem = ::CreateCompatibleDC(dc);
			if(!hmask) {
				Buffer<RGBA> mask(len);
				Buffer<RGBA> bmp(len);
				RGBA *s = buffer;
				RGBA *e = s + len;
				RGBA *b = bmp;
				RGBA *m = mask;
				while(s < e) {
					if(s->a) {
						m->r = m->g = m->b = 0;
						*b = *s;
					}
					else {
						m->r = m->g = m->b = 255;
						b->r = b->g = b->b = 0;
					}
					b->a = m->a = 0;
					s++;
					b++;
					m++;
				}
				BitmapInfo32__ bi(sz.cx, sz.cy);
				HBITMAP o = (HBITMAP)::SelectObject(dcMem, ::CreateDIBitmap(dc, bi, CBM_INIT, mask, bi,
				                                    DIB_RGB_COLORS));
				HDC dcMemMask = ::CreateCompatibleDC(dc);
				if(!hbmp)
					hbmp = ::CreateDIBitmap(dc, bi, CBM_INIT, bmp, bi, DIB_RGB_COLORS);
				hmask = ::CreateBitmap(sz.cx, sz.cy, 1, 1, NULL);
				ResCount += 2;
				::SelectObject(dcMemMask, hmask);
				::SetBkColor(dcMem, RGB(255, 255, 255));
				::BitBlt(dcMemMask, 0, 0, sz.cx, sz.cy, dcMem, 0, 0, SRCCOPY);
				::DeleteObject(::SelectObject(dcMem, o));
				::DeleteDC(dcMemMask);
			}
			HBITMAP o = (HBITMAP)::SelectObject(dcMem, ::CreateCompatibleBitmap(dc, sz.cx, sz.cy));
			::BitBlt(dcMem, 0, 0, sz.cx, sz.cy, dc, x, y, SRCCOPY);
			HDC dcMem2 = ::CreateCompatibleDC(dc);
			::SelectObject(dcMem2, hmask);
			::BitBlt(dcMem, 0, 0, sz.cx, sz.cy, dcMem2, 0, 0, SRCAND);
			if(IsNull(c)) {
				::SelectObject(dcMem2, hbmp);
				::BitBlt(dcMem, 0, 0, sz.cx, sz.cy, dcMem2, 0, 0, SRCPAINT);
			}
			else {
				HBRUSH ho = (HBRUSH) SelectObject(dcMem, CreateSolidBrush(c));
				::BitBlt(dcMem, 0, 0, sz.cx, sz.cy, dcMem2, 0, 0, 0xba0b09);
				::DeleteObject(::SelectObject(dcMem, ho));
			}
			::BitBlt(dc, x, y, sz.cx, sz.cy, dcMem, 0, 0, SRCCOPY);
			::DeleteObject(::SelectObject(dcMem, o));
			::DeleteDC(dcMem2);
			::DeleteDC(dcMem);
		}
		if(hasAlphaBlend && IsNull(c) && !ImageFallBack) {
			if(!himg) {
				BitmapInfo32__ bi(sz.cx, sz.cy);
				himg = CreateDIBSection(ScreenInfo().GetHandle(), bi, DIB_RGB_COLORS,
				                              (void **)&section, NULL, 0);
				ResCount++;
				if(fnAlphaBlend()) {
					RGBA *s = buffer;
					RGBA *e = s + len;
					RGBA *t = section;
					while(s < e) {
						int alpha = s->a + (s->a >> 7);
						t->r = alpha * (s->r) >> 8;
						t->g = alpha * (s->g) >> 8;
						t->b = alpha * (s->b) >> 8;
						t->a = s->a;
						s++;
						t++;
					}
				}
			}
			BLENDFUNCTION bf;
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = const_alpha;
			bf.AlphaFormat = AC_SRC_ALPHA;
			HDC dcMem = ::CreateCompatibleDC(dc);
			::SelectObject(dcMem, himg);
			::fnAlphaBlend()(dc, x, y, sz.cx, sz.cy, dcMem, 0, 0, sz.cx, sz.cy, bf);
			::DeleteDC(dcMem);
		}
		else {
			DrawSurface sf(w, x, y, sz.cx, sz.cy);
			AlphaBlend(sf, buffer, sz.cx * sz.cy, const_alpha, c);
		}
	}
}


void ImageDraw::Section::Init(int cx, int cy)
{
	dc = ::CreateCompatibleDC(ScreenInfo().GetHandle());
	BitmapInfo32__ bi(cx, cy);
	hbmp = CreateDIBSection(dc, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
	hbmpOld = (HBITMAP) ::SelectObject(dc, hbmp);
}

ImageDraw::Section::~Section()
{
	::DeleteObject(::SelectObject(dc, hbmpOld));
	::DeleteDC(dc);
}

void ImageDraw::Init()
{
	rgb.Init(size.cx, size.cy);
	a.Init(size.cx, size.cy);
	Attach(rgb.dc);
	alpha.Attach(a.dc);
}

ImageDraw::operator Image() const
{
	ImageBuffer b(size);
	int n = size.cx * size.cy;
	memcpy(~b, rgb.pixels, n * sizeof(RGBA));
	const RGBA *s = a.pixels;
	const RGBA *e = a.pixels + n;
	RGBA *t = b;
	while(s < e) {
		t->a = s->r;
		t++;
		s++;
	}
	return b;
}

ImageDraw::ImageDraw(Size sz)
{
	size = sz;
	Init();
}

ImageDraw::ImageDraw(int cx, int cy)
{
	size = Size(cx, cy);
	Init();
}

ImageDraw::~ImageDraw()
{
	Detach();
	alpha.Detach();
}

Image Win32Icon(LPCSTR id, bool cursor)
{
	HICON icon;
	if(cursor)
		icon = (HICON)LoadCursor(0, id);
	else
		icon = LoadIcon(0, id);
	ICONINFO iconinfo;
	if(!icon || !GetIconInfo(icon, &iconinfo))
		return Image();
	BITMAP bm;
	::GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm);
	HDC dcMem = ::CreateCompatibleDC(NULL);
	Size sz(bm.bmWidth, bm.bmHeight);
	BitmapInfo32__ bi(sz.cx, sz.cy);
	int len = sz.cx * sz.cy;
	Buffer<RGBA> mask(len);
	::SelectObject(dcMem, iconinfo.hbmColor);
	::GetDIBits(dcMem, iconinfo.hbmMask, 0, sz.cy, ~mask, bi, DIB_RGB_COLORS);
	ImageBuffer b(sz.cx, iconinfo.hbmColor ? sz.cy : sz.cy / 2);
	b.SetHotSpot(Point(iconinfo.xHotspot, iconinfo.yHotspot));
	if(iconinfo.hbmColor) {
		::SelectObject(dcMem, iconinfo.hbmColor);
		::GetDIBits(dcMem, iconinfo.hbmColor, 0, sz.cy, ~b, bi, DIB_RGB_COLORS);
		RGBA *s = ~b;
		RGBA *e = s + len;
		RGBA *m = mask;
		while(s < e) {
			if(s->a != 255 && s->a != 0)
				goto alpha;
			s->a = m->r ? 0 : 255;
			s++;
			m++;
		}
	}
	else {
		len /= 2;
		RGBA *s = ~b;
		RGBA *e = s + len;
		RGBA *c = mask;
		RGBA *m = mask + len;
		while(s < e) {
			s->a = (m->r & ~c->r) ? 0 : 255;
			s->r = s->g = s->b = (c->r & ~m->r) ? 255 : 0;
			s++;
			m++;
			c++;
		}
	}
alpha:
	::DeleteDC(dcMem);
	::DeleteObject(iconinfo.hbmColor);
	::DeleteObject(iconinfo.hbmMask);
	::DestroyIcon(icon);
	return b;
}

HICON IconWin32(Image img, bool cursor)
{
	Size sz = img.GetSize();
	ICONINFO iconinfo;
	iconinfo.fIcon = !cursor;
	Point p = img.GetHotSpot();
	iconinfo.xHotspot = p.x;
	iconinfo.yHotspot = p.y;
	if(IsWinXP() && !ImageFallBack) {
		RGBA *pixels;
		BitmapInfo32__ bi(sz.cx, sz.cy);
		HDC dcMem = ::CreateCompatibleDC(NULL);
		iconinfo.hbmColor = ::CreateDIBSection(dcMem, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
		iconinfo.hbmMask = ::CreateBitmap(sz.cx, sz.cy, 1, 1, NULL);
		memcpy(pixels, ~img, sz.cx * sz.cy * sizeof(RGBA));
		::DeleteDC(dcMem);
	}
	else {
		static Size cursor_size(GetSystemMetrics(SM_CXCURSOR), GetSystemMetrics(SM_CYCURSOR));
		Size tsz = cursor ? cursor_size : sz;
		tsz = cursor_size;
		Size csz = Size(min(tsz.cx, sz.cx), min(tsz.cy, sz.cy));
		Buffer<RGBA>  h(tsz.cx * tsz.cy);
		memset(h, 0, tsz.cx * tsz.cy * sizeof(RGBA));
		int linelen = (tsz.cx + 15) >> 4 << 1;
		Buffer<byte>  mask(tsz.cy * linelen, 0xff);
		byte *m = mask;
		RGBA *ty = h + tsz.cy * tsz.cx;
		const RGBA *sy = ~img + sz.cy * sz.cx;
		for(int y = 0; y < csz.cy; y++) {
			ty -= tsz.cx;
			sy -= sz.cx;
			const RGBA *s = sy;
			RGBA *t = ty;
			for(int x = 0; x < csz.cx; x++) {
				if(((s->a + 50) >> 6) > ((((x & 1) | ((x & 1) << 1)) + ((y & 1) << 1)) & 3)) {
					*t = *s;
					m[x >> 3] &= ~(0x80 >> (x & 7));
				}
				else
					t->r = t->g = t->b = 0;
				t->a = 0;
				t++;
				s++;
			}
			m += linelen;
		}
		HDC dc = ::GetDC(NULL);
		BitmapInfo32__ bi(tsz.cx, tsz.cy);
		iconinfo.hbmColor = ::CreateDIBitmap(dc, bi, CBM_INIT, h, bi, DIB_RGB_COLORS);
		iconinfo.hbmMask = ::CreateBitmap(tsz.cx, tsz.cy, 1, 1, mask);
		::ReleaseDC(NULL, dc);
	}

	HICON icon = ::CreateIconIndirect(&iconinfo);
	::DeleteObject(iconinfo.hbmColor);
	::DeleteObject(iconinfo.hbmMask);
	return icon;
}
