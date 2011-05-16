#include "SystemDraw.h"

NAMESPACE_UPP

#if defined(PLATFORM_WIN32) && defined(flagOPENGL)

#pragma comment( lib, "opengl32.lib" )	// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )		// Search For GLu32.lib While Linking

#define LLOG(x)      // LOG(x)
#define LTIMING(x)   // RTIMING(x)

void planeEquation(double eq[4], float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	eq[0] = y1 * (z2 - z3) + y2*(z3-z1) + y3*(z1-z2);
	eq[1] = (z1*(x2 - x3)) + (z2*(x3 - x1)) + (z3*(x1 - x2));
	eq[2] = (x1*(y2 - y3)) + (x2*(y3 - y1)) + (x3*(y1 - y2));
	eq[3] = -((x1*((y2*z3) - (y3*z2))) + (x2*((y3*z1) - (y1*z3))) + (x3*((y1*z2) - (y2*z1))));
}

void OpenGLDraw::SetClip(const Rect& r)
{
	//glColor4ub(255, 0, 0, 10);
	//glRecti(r.left, r.top, r.right, r.bottom);
	last_clip = r;
	clip = r;
	for(int i = 0; i < ci; i++)
		clip &= cloff[i].drawing_clip;
	
	/*float ix1 = clip.left;
	float iy1 = clip.top;
	float iz = 0;
	float ix2 = clip.right;
	float iy2 = clip.bottom;
	
	double eq[4];
	planeEquation(eq, ix1,iy1,iz, ix1,iy2,iz, ix1,iy2,iz+1.0f);	
	glClipPlane(GL_CLIP_PLANE0, eq);
	glEnable(GL_CLIP_PLANE0);
	
	planeEquation(eq,ix2,iy1,iz, ix2,iy2,iz, ix2,iy2,iz-1.0f);	
	glClipPlane(GL_CLIP_PLANE1, eq);
	glEnable(GL_CLIP_PLANE1);

	planeEquation(eq, ix1,iy1,iz, ix2,iy1,iz, ix2,iy1,iz-1.0f);	
	glClipPlane(GL_CLIP_PLANE2, eq);
	glEnable(GL_CLIP_PLANE2);

	planeEquation(eq, ix1,iy2,iz, ix2,iy2,iz, ix2,iy2,iz+1.0f);	
	glClipPlane(GL_CLIP_PLANE3, eq);
	glEnable(GL_CLIP_PLANE3);*/

	//glScissor(rr.left, drawing_size.cy - rr.top - rr.Height(), rr.Width(), rr.Height());
	
	if(clip_offset.x != 0 || clip_offset.y != 0)
	{
		clip -= clip.TopLeft();
		clip += clip_offset;
	}
}

void OpenGLDraw::BeginOp()
{
	Cloff& w = cloff[ci++];
	w.clipping = false;
	w.org = drawing_offset;
	w.drawing_clip = drawing_clip;	
}

void OpenGLDraw::EndOp()
{
	ASSERT(ci);
	Cloff& w = cloff[--ci];
	drawing_offset = w.org;
	drawing_clip = w.drawing_clip;
	SetClip(drawing_clip);
}

void OpenGLDraw::OffsetOp(Point p)
{
	BeginOp();
	drawing_offset += p;
	SetClip(drawing_clip);
}

bool OpenGLDraw::ClipOp(const Rect& r)
{
	BeginOp();
	drawing_clip = r + drawing_offset;
	SetClip(drawing_clip);
	return true;
}

bool OpenGLDraw::ClipoffOp(const Rect& r)
{
	BeginOp();
	drawing_clip = r + drawing_offset;
	drawing_offset += r.TopLeft();
	SetClip(drawing_clip);
	return true;
}

bool OpenGLDraw::ExcludeClipOp(const Rect& r)
{
	return true;
}

bool OpenGLDraw::IntersectClipOp(const Rect& r)
{
	Cloff& w = cloff[ci];
	w.clipping = true;
	drawing_clip = r + drawing_offset;
	SetClip(drawing_clip);
	return true;
}

bool OpenGLDraw::IsPaintingOp(const Rect& r) const
{
	return true;
}

Rect OpenGLDraw::GetPaintRect() const
{
	return drawing_clip;
}

void OpenGLDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	if(IsNull(color))
		return;
	
	if(cx <= 0 || cy <= 0) return;

	int sx = x + drawing_offset.x;
	int sy = y + drawing_offset.y;
	int dx = sx + cx;
	int dy = sy + cy;

	if(sx > clip.right || sy > clip.bottom)
		return;

	if(dx < clip.left || dy < clip.top)
		return;
	
	if(sx < clip.left)
		sx = clip.left;
	if(sy < clip.top)
		sy = clip.top;
	if(dx > clip.right)
		dx = clip.right;
	if(dy > clip.bottom)
		dy = clip.bottom;
	
	glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
	glRecti(sx, sy, dx, dy);
}

void OpenGLDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	if(cx <= 0 || cy <= 0) return;

	int sx = x + drawing_offset.x;
	int sy = y + drawing_offset.y;
	int dx = sx + cx;
	int dy = sy + cy;

	if(sx > clip.right || sy > clip.bottom)
		return;

	if(dx < clip.left || dy < clip.top)
		return;
	
	float tl = (float) src.left;
	float tr = (float) src.right;
	float tt = (float) src.top;
	float tb = (float) src.bottom;
	
	float sw = (float) src.GetWidth();
	float sh = (float) src.GetHeight();

	if(sx < clip.left)
	{
		int dl = clip.left - sx;
		tl += dl * sw / (float) cx;
		sx = clip.left;
	}
	
	if(sy < clip.top)
	{
		int dt = clip.top - sy;
		tt += dt * sh / (float) cy;
		sy = clip.top;
	}
	
	if(dx > clip.right)
	{
		int dr = dx - clip.right;
		tr -= dr * sw / (float) cx;
		dx = clip.right;
	}
	
	if(dy > clip.bottom)
	{
		int db = dy - clip.bottom;
		tb -= db * sh / (float) cy;
		dy = clip.bottom;
	}
	
	Resources::Bind(img);

	float tw = 1.f / (float) img.GetWidth();
	float th = 1.f / (float) img.GetHeight();

	tl *= tw;
	tr *= tw;
	tt *= th;
	tb *= th;

	if(IsNull(color))	
		glColor4ub(255, 255, 255, (int) alpha);
	else
		glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
	
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glTexCoord2d(tl, tt); glVertex2i(sx, sy);
		glTexCoord2d(tr, tt); glVertex2i(dx, sy);
		glTexCoord2d(tr, tb); glVertex2i(dx, dy);
		glTexCoord2d(tl, tb); glVertex2i(sx, dy);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

void OpenGLDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
	glLineWidth((GLfloat) width);
	glBegin(GL_LINES);
		glVertex2i(x1 + drawing_offset.x, y1 + drawing_offset.y);
		glVertex2i(x2 + drawing_offset.x, y2 + drawing_offset.y);
	glEnd();
}

void OpenGLDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
}

void OpenGLDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
}

void OpenGLDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
}

void OpenGLDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
}

void OpenGLDraw::SaveCurrentColor()
{
	glGetFloatv(GL_CURRENT_COLOR, current_color);
}

void OpenGLDraw::RestoreLastColor()
{
	glColor4f(current_color[0], current_color[1], current_color[2], current_color[3]);
}


#endif

END_UPP_NAMESPACE