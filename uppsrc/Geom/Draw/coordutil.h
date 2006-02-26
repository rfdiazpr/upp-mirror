#ifndef _Geom_Draw_coordutil_h_
#define _Geom_Draw_coordutil_h_

void AlphaTransformPaint(PixelArray& out_pixel, PixelArray& out_alpha,
	const PixelArray& in_pixel, const PixelArray& in_alpha, const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom, const SegmentTreeInfo& info, Color background);

inline void AlphaTransformPaint(AlphaArray& out, const AlphaArray& in, const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom, const SegmentTreeInfo& info, Color background)
{
	AlphaTransformPaint(out.pixel, out.alpha, in.pixel, in.alpha, planar, left, top, right, bottom, info, background);
}

void PaintPlanarTree(Draw& draw, const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom);

void   PlotLine(PlotterTool& tool, GisTransform transform, Pointf source1, Pointf source2);
void   PlotRect(PlotterTool& tool, GisTransform transform, const Rectf& src);

#endif
