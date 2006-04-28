#ifndef __Image__
#define __Image__

#include <Draw/Draw.h>

#ifndef NEWIMAGE
void PixelCopyAntiAlias(PixelArray& dest, Rect dest_rc, const PixelArray& src, Rect src_rc, Rect clip = Null);
void PixelCopyAntiAliasMask(PixelArray& dest, Rect dest_rc,
	const PixelArray& src_pixel, const PixelArray& src_alpha, Rect src_rc, bool inv = false, Rect clip = Null);
inline void PixelCopyAntiAliasMask(PixelArray& dest, Rect dest_rc, const AlphaArray& src,
	Rect src_rc, bool inv = false, Rect clip = Null)
{ PixelCopyAntiAliasMask(dest, dest_rc, src.pixel, src.alpha, src_rc, inv, clip); }
void PixelCopyAntiAliasMaskOut(PixelArray& dest_pixel, PixelArray& dest_alpha, Rect dest_rc,
	const PixelArray& src_pixel, const PixelArray& src_alpha,
	Rect src_rc, bool merge, bool inv = false, Rect clip = Null);
inline void PixelCopyAntiAliasMaskOut(AlphaArray& dest, Rect dest_rc, const AlphaArray& src,
	Rect src_rc, bool merge, bool inv = false, Rect clip = Null)
{ PixelCopyAntiAliasMaskOut(dest.pixel, dest.alpha, dest_rc, src.pixel, src.alpha, src_rc, merge, inv, clip); }
void PixelCopyAntiAliasMaskOnly(PixelArray& dest_alpha, Rect dest_rc,
	const PixelArray& src_alpha, Rect src_rc, bool merge, bool inv = false, Rect clip = Null);

void PixelMapLinear(PixelArray& dest, Point d0, Point d1, Point d2,
	const PixelArray& src, Point s0, Point s1, Point s2, Rect clip = Null, bool antialias = false);
void PixelMapLinearMaskOnly(PixelArray& dest, Point d0, Point d1, Point d2,
	const PixelArray& src, Point s0, Point s1, Point s2, Rect clip = Null, bool antialias = false);
void PixelMapLinearSet(PixelArray& pixel, PixelArray& alpha, Point d0, Point d1, Point d2, Color color, Rect clip = Null);
inline void PixelMapLinearSet(AlphaArray& array, Point d0, Point d1, Point d2, Color color, Rect clip = Null)
{ PixelMapLinearSet(array.pixel, array.alpha, d0, d1, d2, color, clip); }

void PixelMapLinearMask(PixelArray& dest, Point d0, Point d1, Point d2,
	const PixelArray& src_pixel, const PixelArray& src_alpha, Point s0, Point s1, Point s2,
	Rect clip = Null, bool antialias = false);
inline void PixelMapLinearMask(PixelArray& dest, Point d0, Point d1, Point d2,
	const AlphaArray& src, Point s0, Point s1, Point s2,
	Rect clip = Null, bool antialias = false)
{ PixelMapLinearMask(dest, d0, d1, d2, src.pixel, src.alpha, s0, s1, s2, clip, antialias); }

void PixelMapLinearMaskOut(PixelArray& dest_pixel, PixelArray& dest_alpha, Point d0, Point d1, Point d2,
	const PixelArray& src_pixel, const PixelArray& src_alpha,
	Point s0, Point s1, Point s2, Rect clip = Null, bool antialias = false);
inline void PixelMapLinearMaskOut(AlphaArray& dest, Point d0, Point d1, Point d2,
	const AlphaArray& src, Point s0, Point s1, Point s2, Rect clip = Null, bool antialias = false)
{ PixelMapLinearMaskOut(dest.pixel, dest.alpha, d0, d1, d2, src.pixel, src.alpha, s0, s1, s2, clip, antialias); }

void PixelMapBilinear(PixelArray& dest, const Point destpos[4], const PixelArray& src, const Point srcpos[4], Rect clip = Null, bool antialias = false);
void PixelMapBilinear(PixelArray& dest, const Point destpos[4], const PixelArray& src, Rect srcrc, Rect clip = Null, bool antialias = false);
void PixelMapBilinearMaskOnly(PixelArray& dest, const Point destpos[4], const PixelArray& src, const Point srcpos[4], Rect clip = Null, bool antialias = false);
void PixelMapBilinearMaskOnly(PixelArray& dest, const Point destpos[4], const PixelArray& src, Rect srcrc, Rect clip = Null, bool antialias = false);
void PixelMapBilinearSet(PixelArray& pixel, PixelArray& alpha, const Point destpos[4], Color color, Rect clip = Null);
inline void PixelMapBilinearSet(AlphaArray& array, const Point destpos[4], Color color, Rect clip = Null)
{ PixelMapBilinearSet(array.pixel, array.alpha, destpos, color, clip); }

void PixelMapBilinearMask(PixelArray& dest, const Point destpos[4], const PixelArray& src_pixel, const PixelArray& src_alpha,
	const Point srcpos[4], Rect clip = Null, bool antialias = false);
void PixelMapBilinearMask(PixelArray& dest, const Point destpos[4], const PixelArray& src_pixel, const PixelArray& src_alpha,
	Rect srcrc, Rect clip = Null, bool antialias = false);
inline void PixelMapBilinearMask(PixelArray& dest, const Point destpos[4], const AlphaArray& src,
	const Point srcpos[4], Rect clip = Null, bool antialias = false)
{ PixelMapBilinearMask(dest, destpos, src.pixel, src.alpha, srcpos, clip, antialias); }
inline void PixelMapBilinearMask(PixelArray& dest, const Point destpos[4], const AlphaArray& src,
	Rect srcrc, Rect clip = Null, bool antialias = false)
{ PixelMapBilinearMask(dest, destpos, src.pixel, src.alpha, srcrc, clip, antialias); }

void PixelMapBilinearMaskOut(PixelArray& dest_pixel, PixelArray& dest_alpha, const Point destpos[4],
	const PixelArray& src_pixel, const PixelArray& src_alpha, const Point srcpos[4],
	Rect clip = Null, bool antialias = false);
void PixelMapBilinearMaskOut(PixelArray& dest_pixel, PixelArray& dest_alpha, const Point destpos[4],
	const PixelArray& src_pixel, const PixelArray& src_alpha, Rect srcrc,
	Rect clip = Null, bool antialias = false);
inline void PixelMapBilinearMaskOut(AlphaArray& dest, const Point destpos[4], const AlphaArray& src,
	const Point srcpos[4], Rect clip = Null, bool antialias = false)
{ PixelMapBilinearMaskOut(dest.pixel, dest.alpha, destpos, src.pixel, src.alpha, srcpos, clip, antialias); }
inline void PixelMapBilinearMaskOut(AlphaArray& dest, const Point destpos[4], const AlphaArray& src,
	Rect srcrc, Rect clip = Null, bool antialias = false)
{ PixelMapBilinearMaskOut(dest.pixel, dest.alpha, destpos, src.pixel, src.alpha, srcrc, clip, antialias); }
#endif

#endif//__Image__
