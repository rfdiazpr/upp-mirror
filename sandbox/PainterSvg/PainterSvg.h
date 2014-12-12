#ifndef _PainterSvg_PainterSvg_h
#define _PainterSvg_PainterSvg_h

#include <Painter/Painter.h>

NAMESPACE_UPP

#include "Svg.h"

bool  ParseSVG(Painter& p, const char *svg, const char *folder);
Rectf GetSvgSize(const char *svg);

END_UPP_NAMESPACE

#endif

