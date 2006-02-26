//////////////////////////////////////////////////////////////////////
// imageio_palette.cpp: image palete mapper.

#include "Draw.h"

#if defined(PLATFORM_WIN32) && defined(QLIB2IMAGE)

#ifdef CPU_IA32
#define I386_ASSEMBLY
#endif

#ifdef I386_ASSEMBLY
#define BLT_TIMING(routine) //RTIMING(#routine " (asm)")
#else
#define BLT_TIMING(routine) //RTIMING(#routine " (cpp)")
#endif

enum
{
	R_MAP_BITS = 4,
	G_MAP_BITS = 4,
	B_MAP_BITS = 4,
};

//////////////////////////////////////////////////////////////////////

void BltXlatRgb(byte *dest, const byte *src, unsigned count, const byte *table)
{
	BLT_TIMING(BltXlatRgb)

#ifdef I386_ASSEMBLY
	__asm mov   esi, [src]
	__asm mov   edi, [dest]
	__asm mov   ebx, [table]
	__asm mov   ecx, [count]
	__asm shr   ecx, 3
	__asm je    __1
__2:
#define DO_2_PIXELS(o) \
	__asm movzx eax, byte ptr [esi + 3 * o + 2] \
	__asm movzx edx, byte ptr [esi + 3 * o + 5] \
	__asm shl   eax, R_MAP_BITS \
	__asm shl   edx, R_MAP_BITS \
	__asm mov   al, [esi + 3 * o + 1] \
	__asm mov   dl, [esi + 3 * o + 4] \
	__asm shl   eax, G_MAP_BITS \
	__asm shl   edx, G_MAP_BITS \
	__asm mov   al, [esi + 3 * o + 0] \
	__asm mov   dl, [esi + 3 * o + 3] \
	__asm shr   eax, 8 - B_MAP_BITS \
	__asm shr   edx, 8 - B_MAP_BITS \
	__asm mov   al, [eax + ebx] \
	__asm mov   dl, [edx + ebx] \
	__asm mov   [edi + o + 0], al \
	__asm mov   [edi + o + 1], dl \

	DO_2_PIXELS(0)
	DO_2_PIXELS(2)
	DO_2_PIXELS(4)
	DO_2_PIXELS(6)
	__asm add   esi, 3 * 8
	__asm add   edi, 8
	__asm dec   ecx
	__asm jne   __2
__1:
	__asm mov   ecx, [count]
	__asm test  cl, 4
	__asm je    __3

	DO_2_PIXELS(0)
	DO_2_PIXELS(2)
	__asm add   esi, 3 * 4
	__asm add   edi, 4
__3:
	__asm test  cl, 2
	__asm je    __4
	
	DO_2_PIXELS(0)
#undef DO_2_PIXELS
	__asm add   esi, 3 * 2
	__asm add   edi, 2
__4:
	__asm test  cl, 1
	__asm je    __5
	__asm movzx eax, byte ptr [esi + 2]
	__asm shl   eax, R_MAP_BITS
	__asm mov   al, [esi + 1]
	__asm shl   eax, G_MAP_BITS
	__asm mov   al, [esi]
	__asm shr   eax, 8 - B_MAP_BITS
	__asm mov   al, [eax + ebx]
	__asm mov   [edi], al
__5:
	;
#else
	while(count >= 4)
	{
#define DO_PIXEL(o) \
		dest[o] = table[((src[3 * o + 2] >> (8 - R_MAP_BITS)) << (G_MAP_BITS + B_MAP_BITS)) \
			|           ((src[3 * o + 1] >> (8 - G_MAP_BITS)) << (B_MAP_BITS)) \
			|            (src[3 * o + 0] >> (8 - B_MAP_BITS))];
		DO_PIXEL(0)
		DO_PIXEL(1)
		DO_PIXEL(2)
		DO_PIXEL(3)
		src += 3 * 4;
		dest += 4;
		count -= 4;
	}
	if(count & 2)
	{
		DO_PIXEL(0)
		DO_PIXEL(1)
		src += 3 * 2;
		dest += 2;
	}
	if(count & 1)
	{
		DO_PIXEL(0)
#undef DO_PIXEL
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void BltStatRgb(const byte *p, unsigned count, dword *stat)
{
	BLT_TIMING(BltStatRgb)

#ifdef I386_ASSEMBLY
	__asm mov    esi, [p]
	__asm mov    ecx, [count]
	__asm mov    ebx, [stat]
	__asm shr    ecx, 3
	__asm je     __1
__2:
#define DO_2_PIXELS(o) \
	__asm movzx  eax, byte ptr [esi + 3 * o + 2] \
	__asm movzx  edx, byte ptr [esi + 3 * o + 5] \
	__asm shl    eax, R_MAP_BITS \
	__asm shl    edx, R_MAP_BITS \
	__asm mov    al, [esi + 3 * o + 1] \
	__asm mov    dl, [esi + 3 * o + 4] \
	__asm shl    eax, G_MAP_BITS \
	__asm shl    edx, G_MAP_BITS \
	__asm mov    al, [esi + 3 * o + 0] \
	__asm mov    dl, [esi + 3 * o + 3] \
	__asm shr    eax, 8 - B_MAP_BITS \
	__asm shr    edx, 8 - B_MAP_BITS \
	__asm inc    dword ptr [ebx + 4 * eax] \
	__asm inc    dword ptr [ebx + 4 * edx]

	DO_2_PIXELS(0)
	DO_2_PIXELS(2)
	DO_2_PIXELS(4)
	DO_2_PIXELS(6)
	__asm add    esi, 3 * 8
	__asm dec    ecx
	__asm jne    __2
__1:
	__asm mov    ecx, [count]
	__asm test   cl, 4
	__asm je     __3

	DO_2_PIXELS(0)
	DO_2_PIXELS(2)
	__asm add    esi, 3 * 4
__3:
	__asm test   cl, 2
	__asm je     __4

	DO_2_PIXELS(0)
#undef DO_2_PIXELS
	__asm add    esi, 3 * 2
__4:
	__asm test   cl, 1
	__asm je     __9
	__asm movzx  eax, byte ptr [esi + 2]
	__asm shl    eax, R_MAP_BITS
	__asm mov    al, [esi + 1]
	__asm shl    eax, G_MAP_BITS
	__asm mov    al, [esi]
	__asm shr    eax, 8 - B_MAP_BITS
	__asm inc    dword ptr [ebx + 4 * eax]
__9:
	;
#else
	while(count >= 4)
	{
#define DO_PIXEL(o) \
		stat[ ((p[3 * o + 2] >> (8 - R_MAP_BITS)) << (G_MAP_BITS + B_MAP_BITS)) \
			| ((p[3 * o + 1] >> (8 - G_MAP_BITS)) << (B_MAP_BITS)) \
			|  (p[3 * o + 0] >> (8 - B_MAP_BITS))]++;
		DO_PIXEL(0)
		DO_PIXEL(1)
		DO_PIXEL(2)
		DO_PIXEL(3)
		p += 3 * 4;
		count -= 4;
	}
	if(count & 2)
	{
		DO_PIXEL(0)
		DO_PIXEL(1)
		p += 3 * 2;
	}
	if(count & 1)
	{
		DO_PIXEL(0)
	}
#endif
}

//////////////////////////////////////////////////////////////////////

class ImagePaletteMapper
{
public:
	ImagePaletteMapper(RawImage& out_image, const RawImage& image, int out_bpp = 8, int colors = 256, const Color *palette = 0);

	void          Run();

private:
	void          CreateMapping(int r1, int r2, int g1, int g2, int b1, int b2, int avail);
	void          CreateIndex();
	int           GetDist(int r, int g, int b, int color) const
	{
		r -= GetRValue(color);
		g -= GetGValue(color);
		b -= GetBValue(color);
		return r * r + g * g + b * b;
	}

private:
	RawImage&     out_image;
	const RawImage& image;
	int           row_bytes;
	int           out_bytes;
	const Color  *palette;
	int           palette_count;
	int           out_bpp;

	enum Consts {
		R_MAP    = 1 << R_MAP_BITS,
		G_MAP    = 1 << G_MAP_BITS,
		B_MAP    = 1 << B_MAP_BITS,
		R_LOSE   = 8 - R_MAP_BITS,
		G_LOSE   = 8 - G_MAP_BITS,
		B_LOSE   = 8 - B_MAP_BITS,
		R_LOSE_1 = R_LOSE - 1,
		G_LOSE_1 = G_LOSE - 1,
		B_LOSE_1 = B_LOSE - 1,
		R_MUL    = 1 << R_LOSE,
		G_MUL    = 1 << G_LOSE,
		B_MUL    = 1 << B_LOSE,
		R_MUL_1  = 1 << R_LOSE_1,
		G_MUL_1  = 1 << G_LOSE_1,
		B_MUL_1  = 1 << B_LOSE_1,
		R_ADD    = R_MUL - 1,
		G_ADD    = G_MUL - 1,
		B_ADD    = B_MUL - 1,
		R_ADD_1  = 2 * R_MUL_1 - 1,
		G_ADD_1  = 2 * G_MUL_1 - 1,
		B_ADD_1  = 2 * B_MUL_1 - 1,
	};

	dword         stat[R_MAP][G_MAP][B_MAP];
	byte          map[R_MAP][G_MAP][B_MAP];
	int           val_r[R_MAP], val_g[G_MAP], val_b[B_MAP];
	int           oldcnt;
	int           palstat[256];
	byte          palmap[256];
};

void CreatePalette(RawImage& out_image, const RawImage& image, int out_bpp, int colors, const Color *palette)
{
	ImagePaletteMapper(out_image, image, out_bpp, colors, palette).Run();
}

/*
Vector<Color> ImageCreatePalette(byte *ptr, Size size, const Color *palette, int colors)
{ 
//	TIMING("ImageCreatePalette");
	return ImagePaletteMapper(ptr, size, palette, colors).Run(); 
}
*/

//////////////////////////////////////////////////////////////////////

ImagePaletteMapper::ImagePaletteMapper(RawImage& out_image, const RawImage& image, int out_bpp, int colors, const Color *_palette)
: out_image(out_image)
, image(image)
, out_bpp(out_bpp)
, palette(_palette)
, palette_count(colors)
{
	ASSERT(out_image && image && (image.bpp == 24 || image.bpp == 8) && (out_bpp == 1 || out_bpp == 4 || out_bpp == 8));
	row_bytes = image.GetRowBytes();
	out_bytes = ((image.size.cx * out_bpp + 31) >> 3) & -4;
	if(colors <= 2 && palette == 0)
	{
		static const Color bw[] = { Black, White };
		palette = bw;
	}
}

//////////////////////////////////////////////////////////////////////

void ImagePaletteMapper::Run()
{
	if(image.bpp <= 8 && image.palette.GetCount() <= palette_count && palette == 0)
		return;

	ZeroArray(map);

	Vector<Color> old_palette(image.palette, 0);
	out_image.palette.Clear();
	if(palette)
	{ // use given palette colors
		Append(out_image.palette, palette, palette + palette_count);
		memset(&stat, 1, sizeof(stat));
	}
	else
	{ // gather statistics & create mapping cube
		ZeroArray(stat);
		if(image.bpp == 24)
		{
			const byte *row = image.data.Begin();
			for(int y = 0; y < image.size.cy; y++, row += row_bytes)
				BltStatRgb(row, image.size.cx, &stat[0][0][0]);
		}
		else
		{
			dword counts[256];
			ZeroArray(counts);
			for(int y = 0; y < image.size.cy; y++)
				BltStatB(image.GetUpScan(y), image.size.cx, counts);
			for(int i = 0; i < old_palette.GetCount(); i++)
			{
				Color c = old_palette[i];
				stat[c.GetR() >> R_LOSE][c.GetG() >> G_LOSE][c.GetB() >> B_LOSE] += counts[i];
			}
		}
		CreateMapping(0, R_MAP - 1, 0, G_MAP - 1, 0, B_MAP - 1, palette_count);
	}
	out_image.palette.Shrink();
	CreateIndex();

	Vector<byte> out_buffer;
	if(out_bpp < 8)
		out_buffer.SetCount((image.size.cx + 3) & -4);
	void (*pack)(byte *, const byte *, unsigned) = (out_bpp == 8 ? 0 : out_bpp == 4 ? Blt8to4 : Blt8to1);

	if(image.bpp == 24)
	{ // remap image
		byte *dest = out_image.data.Begin();
		for(const byte *src = image.data.Begin(), *send = src + image.size.cy * row_bytes; src < send; src += row_bytes, dest += out_bytes)
			if(pack)
			{
				BltXlatRgb(out_buffer.Begin(), src, image.size.cx, &map[0][0][0]);
				pack(dest, out_buffer.Begin(), image.size.cx);
			}
			else
				BltXlatRgb(dest, src, image.size.cx, &map[0][0][0]);
	}
	else
	{
		ASSERT(image.bpp == 8);
		byte table[256];
		ZeroArray(table);
		for(int i = 0; i < old_palette.GetCount(); i++)
		{
			Color c = old_palette[i];
			table[i] = map[c.GetR() >> R_LOSE][c.GetG() >> G_LOSE][c.GetB() >> B_LOSE];
		}
		byte *dest = out_image.data.Begin();
		const byte *src = image.data.Begin();
		for(int y = 0; y < image.size.cy; y++, src += row_bytes, dest += out_bytes)
			if(pack)
			{
				BltXlatB(out_buffer.Begin(), src, image.size.cx, table);
				pack(dest, out_buffer.Begin(), image.size.cx);
			}
			else
				BltXlatB(dest, src, image.size.cx, table);
	}

	out_image.bpp = 8;
	out_image.row_bytes = out_bytes;
}

//////////////////////////////////////////////////////////////////////

void ImagePaletteMapper::CreateMapping(int r1, int r2, int g1, int g2, int b1, int b2, int avail)
{
	if(r1 > r2 || g1 > g2 || b1 > b2)
		return;
	ASSERT(r1 >= 0 && r2 < R_MAP && g1 >= 0 && g2 < G_MAP && b1 >= 0 && b2 < B_MAP);
	ZeroArray(val_r);
	ZeroArray(val_g);
	ZeroArray(val_b);
	int full = 0;
	int total = 0;
	{ // gather statistics
		for(int r = r1; r <= r2; r++)
			for(int g = g1; g <= g2; g++)
				for(int b = b1; b <= b2; b++)
				{
					int value = stat[r][g][b];
					if(value)
					{
						val_r[r] += value;
						val_g[g] += value;
						val_b[b] += value;
						full++;
						total += value;
					}
				}
	}

	// limit ranges
	while(r1 < r2 && val_r[r1] == 0)
		r1++;
	while(r2 > r1 && val_r[r2] == 0)
		r2--;
	while(g1 < g2 && val_g[g1] == 0)
		g1++;
	while(g2 > g1 && val_g[g2] == 0)
		g2--;
	while(b1 < b2 && val_b[b1] == 0)
		b1++;
	while(b2 > b1 && val_b[b2] == 0)
		b2--;

	if(full <= avail)
	{ // not many colors are needed; map them all
		for(int r = r1; r <= r2; r++)
			for(int g = g1; g <= g2; g++)
				for(int b = b1; b <= b2; b++)
					if(stat[r][g][b])
					{
						map[r][g][b] = out_image.palette.GetCount();
						out_image.palette.Add(Color(R_MUL * r + R_ADD, G_MUL * g + G_ADD, B_MUL * b + B_ADD));
					}
		ASSERT(out_image.palette.GetCount() <= palette_count);
		return;
	}

	if(avail <= 1)
	{ // there is just 1 color available - assign it to all candidates
		byte i = out_image.palette.GetCount();
		out_image.palette.Add(Color(
			((R_MUL * (r1 + r2)) >> 1) + R_ADD, 
			((G_MUL * (g1 + g2)) >> 1) + G_ADD, 
			((B_MUL * (b1 + b2)) >> 1) + B_ADD));
		for(int r = r1; r <= r2; r++)
			for(int g = g1; g <= g2; g++)
				memset(&map[r][g][b1], i, b2 - b1 + 1);
		return;
	}

	int dr = r2 - r1, dg = g2 - g1, db = b2 - b1;
	int top = out_image.palette.GetCount() + avail;
	if(dg >= dr && dg >= db)
	{ // split green
		int mid = (r1 + r2) >> 1;
		CreateMapping(r1, mid + 0, g1, g2, b1, b2, (top - out_image.palette.GetCount()) >> 1);
		CreateMapping(mid + 1, r2, g1, g2, b1, b2, top - out_image.palette.GetCount());
	}
	else if(dr >= db)
	{ // split red
		int mid = (g1 + g2) >> 1;
		CreateMapping(r1, r2, g1, mid + 0, b1, b2, (top - out_image.palette.GetCount()) >> 1);
		CreateMapping(r1, r2, mid + 1, g2, b1, b2, top - out_image.palette.GetCount());
	}
	else
	{ // split blue
		int mid = (b1 + b2) >> 1;
		CreateMapping(r1, r2, g1, g2, b1, mid + 0, (top - out_image.palette.GetCount()) >> 1);
		CreateMapping(r1, r2, g1, g2, mid + 1, b2, top - out_image.palette.GetCount());
	}
	ASSERT(out_image.palette.GetCount() <= 256);
}

//////////////////////////////////////////////////////////////////////

void ImagePaletteMapper::CreateIndex()
{
	ASSERT(out_image.palette.GetCount() <= 256);

	enum { TOLERANCE = 27 };

	int r_index[256], g_index[256], b_index[256], c_index[256];
	int cc = out_image.palette.GetCount();
	if(cc <= 1)
	{
		ZeroArray(map);
		return;
	}
	for(int i = 0; i < cc; i++)
	{
		Color c = out_image.palette[i];
		r_index[i] = (c.GetR() << 8) | i;
		g_index[i] = (c.GetG() << 8) | i;
		b_index[i] = (c.GetB() << 8) | i;
		c_index[i] = RGB(c.GetR(), c.GetG(), c.GetB());
	}

	int *r_last = r_index + (cc - 1);
	int *g_last = g_index + (cc - 1);
	int *b_last = b_index + (cc - 1);

	Sort(r_index, r_last + 1, StdLess<int>());
	Sort(g_index, g_last + 1, StdLess<int>());
	Sort(b_index, b_last + 1, StdLess<int>());

	for(int r = 0; r < R_MAP; r++)
	{
		int rv = r * R_MUL + R_ADD;
		const int *rx = BinFind(r_index, r_last, rv << 8, StdLess<int>());
		for(int g = 0; g < G_MAP; g++)
		{
			int gv = g * G_MUL + G_ADD;
			const int *gx = BinFind(g_index, g_last, gv << 8, StdLess<int>());
			for(int b = 0; b < B_MAP; b++)
			{
				if(stat[r][g][b] == 0)
					continue;
				int bv = b * B_MUL + B_ADD;
				const int *bx = BinFind(b_index, b_last, bv << 8, StdLess<int>());
				int d2g = GetDist(rv, gv, bv, c_index[*gx & 0xFF]);
				if(d2g <= TOLERANCE)
				{
					map[r][g][b] = *gx & 0xFF;
					continue;
				}
				int d2r = (*gx ^ *rx) & 0xFF ? GetDist(rv, gv, bv, c_index[*rx & 0xFF]) : d2g;
				if(d2r <= TOLERANCE)
				{
					map[r][g][b] = *rx & 0xFF;
					continue;
				}
				int d2b = (*gx ^ *bx) & 0xFF ? (*rx ^ *bx) & 0xFF ? 
					GetDist(rv, gv, bv, c_index[*bx & 0xFF]) : d2r : d2g;
				if(d2b <= TOLERANCE)
				{
					map[r][g][b] = *bx & 0xFF;
					continue;
				}
				const int *s, *p, *l, *t;
				int best, level;
				if(d2g <= d2r && d2g <= d2b)
				{ // green stepping
					s = g_index;
					p = gx;
					l = g_last;
					best = d2g;
					level = gv;
				}
				else if(d2r <= d2b)
				{
					s = r_index;
					p = rx;
					l = r_last;
					best = d2r;
					level = rv;
				}
				else
				{
					s = b_index;
					p = bx;
					l = b_last;
					best = d2b;
					level = bv;
				}
				byte match = *p & 0xFF;
				if((t = p) > s)
				{
					while(sqr((t[-1] >> 8) - level) < best)
					{
						int d = GetDist(rv, gv, bv, c_index[*--t & 0xFF]);
						if(d < best)
						{
							match = *t & 0xFF;
							if((best = d) <= TOLERANCE)
								break;
						}
						if(t == s)
							break;
					}
				}
				if(best > TOLERANCE && (t = p) < l)
				{
					while(sqr((t[1] >> 8) - level) < best)
					{
						int d = GetDist(rv, gv, bv, c_index[*++t & 0xFF]);
						if(d < best)
						{
							match = *t & 0xFF;
							if((best = d) <= TOLERANCE)
								break;
						}
						if(t == l)
							break;
					}
				}
				map[r][g][b] = match;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
#endif
