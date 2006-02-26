//////////////////////////////////////////////////////////////////////
// imageio_blt.cpp: accelerator routines.

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

//////////////////////////////////////////////////////////////////////

bool MMXDetect()
{
	static bool mmx = IsProcessorFeaturePresent(PF_MMX_INSTRUCTIONS_AVAILABLE);
	return mmx;
}

//////////////////////////////////////////////////////////////////////

void MMXClear()
{
#if defined(CPU_IA32) && defined(COMPILER_MSC)
	if(MMXDetect())
		__asm emms
#endif
}

//////////////////////////////////////////////////////////////////////

#ifdef I386_ASSEMBLY
static void MMXDumpRaw()
{
	byte mreg[8][8];
	__asm movq [mreg +  0], mm0
	__asm movq [mreg +  8], mm1
	__asm movq [mreg + 16], mm2
	__asm movq [mreg + 24], mm3
	__asm movq [mreg + 32], mm4
	__asm movq [mreg + 40], mm5
	__asm movq [mreg + 48], mm6
	__asm movq [mreg + 56], mm7
	__asm emms

	for(int m = 0; m < 8; m++)
	{
		String out;
		out << "mm" << m << " =";
		const byte *b = mreg[m];
		
		for(int t = 7; t >= 0; t--)
			out << Format(" %02x", b[t]);
		RLOG(out);
		puts(out);
	}
	puts("");
	
	__asm movq mm0, [mreg +  0]
	__asm movq mm1, [mreg +  8]
	__asm movq mm2, [mreg + 16]
	__asm movq mm3, [mreg + 24]
	__asm movq mm4, [mreg + 32]
	__asm movq mm5, [mreg + 40]
	__asm movq mm6, [mreg + 48]
	__asm movq mm7, [mreg + 56]
}

void __declspec(naked) MMXDump()
{
	__asm push eax
	__asm push ebx
	__asm push ecx
	__asm push edx
	MMXDumpRaw();
	__asm pop edx
	__asm pop ecx
	__asm pop ebx
	__asm pop eax
	__asm ret
}

#endif//I386_ASSEMBLY

//////////////////////////////////////////////////////////////////////
// blitter routines

void BltXlatB(byte *dest, const byte *src, unsigned count, const byte *table)
{
	BLT_TIMING(BltXlatB)

#ifdef I386_ASSEMBLY
	__asm mov    ecx, [count]
	__asm mov    edx, [table]
	__asm xor    ebx, ebx
	__asm mov    esi, [src]
	__asm mov    edi, [dest]
	__asm cmp    ecx, 8
	__asm jb     __1
	__asm shr    ecx, 3
#define DO_4_PIXELS(delta) \
	__asm mov    eax, [esi + delta] \
	__asm mov    bl, al \
	__asm mov    al, [edx + ebx] \
	__asm mov    bl, ah \
	__asm mov    ah, [edx + ebx] \
	__asm ror    eax, 16 \
	__asm mov    bl, al \
	__asm mov    al, [edx + ebx] \
	__asm mov    bl, ah \
	__asm mov    ah, [edx + ebx] \
	__asm ror    eax, 16 \
	__asm mov    [edi + delta], eax
__2:
	DO_4_PIXELS(0)
	DO_4_PIXELS(4)
	__asm add    esi, 8
	__asm add    edi, 8
	__asm dec    ecx
	__asm jne    __2
	__asm mov    ecx, [count]
__1:
	__asm test   cl, 4
	__asm je     __3
	DO_4_PIXELS(0)
#undef DO_4_PIXELS
	__asm add    esi, 4
	__asm add    edi, 4
__3:
	__asm test   cl, 2
	__asm je     __4
	__asm mov    ax, [esi]
	__asm mov    bl, al
	__asm mov    al, [edx + ebx]
	__asm mov    bl, ah
	__asm mov    ah, [edx + ebx]
	__asm mov    [edi], ax
	__asm add    esi, 2
	__asm add    edi, 2
__4:
	__asm test   cl, 1
	__asm je     __9
	__asm mov    bl, [esi]
	__asm mov    al, [edx + ebx]
	__asm mov    [edi], al
__9:
	;
#else
	while(count >= 4)
	{
		dest[0] = table[src[0]];
		dest[1] = table[src[1]];
		dest[2] = table[src[2]];
		dest[3] = table[src[3]];
		src += 4;
		dest += 4;
		count -= 4;
	}
	if(count & 2)
	{
		dest[0] = table[src[0]];
		dest[1] = table[src[1]];
		src += 2;
		dest += 2;
	}
	if(count & 1)
		dest[0] = table[src[0]];
#endif
}

//////////////////////////////////////////////////////////////////////

void BltStatB(const byte *p, unsigned count, dword *stat)
{
	BLT_TIMING(BltStatB)

#ifdef I386_ASSEMBLY
	__asm xor   eax, eax
	__asm xor   ebx, ebx
	__asm mov   ecx, [count]
	__asm mov   edx, [stat]
	__asm mov   esi, [p]
	__asm cmp   ecx, 8
	__asm jb    __1
	__asm shr   ecx, 3
#define DO_2_PIXELS(delta) \
	__asm mov   al, [esi + delta + 0] \
	__asm mov   bl, [esi + delta + 1] \
	__asm inc   dword ptr [edx + 4 * eax] \
	__asm inc   dword ptr [edx + 4 * ebx]
__2:
	DO_2_PIXELS(0)
	DO_2_PIXELS(2)
	DO_2_PIXELS(4)
	DO_2_PIXELS(6)
	__asm add   esi, 8
	__asm dec   ecx
	__asm jne   __2
	__asm mov   ecx, [count]
__1:
	__asm test  cl, 4
	__asm je    __3
	DO_2_PIXELS(0)
	DO_2_PIXELS(2)
	__asm add   esi, 4
__3:
	__asm test  cl, 2
	__asm je    __4
	DO_2_PIXELS(0)
#undef DO_2_PIXELS
	__asm add   esi, 2
__4:
	__asm test  cl, 1
	__asm je    __9
	__asm mov   al, [esi]
	__asm inc   dword ptr [edx + 4 * eax]
__9:
	;
#else
	while(count >= 8)
	{
		stat[p[0]]++;
		stat[p[1]]++;
		stat[p[2]]++;
		stat[p[3]]++;
		stat[p[4]]++;
		stat[p[5]]++;
		stat[p[6]]++;
		stat[p[7]]++;
		p += 8;
		count -= 8;
	}
	if(count & 4)
	{
		stat[p[0]]++;
		stat[p[1]]++;
		stat[p[2]]++;
		stat[p[3]]++;
		p += 4;
	}
	if(count & 2)
	{
		stat[p[0]]++;
		stat[p[1]]++;
		p += 2;
	}
	if(count & 1)
		stat[p[0]]++;
#endif
}

//////////////////////////////////////////////////////////////////////

static void Break()
{
	__asm int 3
}

//////////////////////////////////////////////////////////////////////

void BltSwapRGB(byte *d, const byte *s, unsigned count)
{
#define BLT_SWAP_RGB_2(o) \
	a = s[3 * (o) + 0]; b = s[3 * (o) + 1]; \
	d[3 * (o) + 2] = a; d[3 * (o) + 1] = b; \
	a = s[3 * (o) + 2]; b = s[3 * (o) + 3]; \
	d[3 * (o) + 0] = a; d[3 * (o) + 5] = b; \
	a = s[3 * (o) + 4]; b = s[3 * (o) + 5]; \
	d[3 * (o) + 4] = a; d[3 * (o) + 3] = b; \
	
#define BLT_SWAP_RGB_8(o) BLT_SWAP_RGB_2((o)) BLT_SWAP_RGB_2((o) + 2) BLT_SWAP_RGB_2((o) + 4) BLT_SWAP_RGB_2((o) + 6)
	
//	Break();
	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_SWAP_RGB_8(0) BLT_SWAP_RGB_8(8) BLT_SWAP_RGB_8(16) BLT_SWAP_RGB_8(24)
		s += 32 * 3;
		d += 32 * 3;
	}
	if(count & 16)
	{
		BLT_SWAP_RGB_8(0) BLT_SWAP_RGB_8(8)
		s += 16 * 3;
		d += 16 * 3;
	}
	if(count & 8)
	{
		BLT_SWAP_RGB_8(0)
		s += 8 * 3;
		d += 8 * 3;
	}
	if(count & 4)
	{
		BLT_SWAP_RGB_2(0) BLT_SWAP_RGB_2(2)
		s += 4 * 3;
		d += 4 * 3;
	}
	if(count & 2)
	{
		BLT_SWAP_RGB_2(0)
		s += 2 * 3;
		d += 2 * 3;
	}
	if(count & 1)
	{
		d[0] = s[2]; d[1] = s[1]; d[2] = s[0];
	}
}

//////////////////////////////////////////////////////////////////////

void BltAverage(byte *p, unsigned count, const byte *r, const byte *g, const byte *b)
{
	BLT_TIMING(BltAverage)

#ifdef I386_ASSEMBLY
	enum
	{
		ACCEL_BITS = 3,
		ACCEL_COUNT = 1 << (int)ACCEL_BITS
	};
	__asm mov    ebx, [p]
	__asm mov    edx, [b]
	__asm mov    esi, [g]
	__asm mov    edi, [r]
	__asm xor    eax, eax
	__asm xor    ecx, ecx
	__asm sub    [count], ACCEL_COUNT
	__asm jb     __1
__2:
#define PAIR(i1, i2, r1, r2) \
	__asm mov    al, [ebx + (i1)] \
	__asm mov    cl, [ebx + (i2)] \
	__asm mov    al, [eax + r1] \
	__asm mov    cl, [ecx + r2] \
	__asm mov    [ebx + (i1)], al \
	__asm mov    [ebx + (i2)], cl

#define PIXEL2(i) \
	PAIR(3 * (i) + 0, 3 * (i) + 1, edx, esi) \
	PAIR(3 * (i) + 2, 3 * (i) + 3, edi, edx) \
	PAIR(3 * (i) + 4, 3 * (i) + 5, esi, edi)
	
	PIXEL2(0)
	PIXEL2(2)
	PIXEL2(4)
	PIXEL2(6)
#undef PIXEL2
#undef PAIR

	__asm add    ebx, ACCEL_COUNT * 3
	__asm sub    [count], ACCEL_COUNT
	__asm jnb    __2
__1:
	__asm mov    ecx, [count]
	__asm and    ecx, (ACCEL_COUNT - 1)
	__asm je     __9
__3:
	__asm mov    al, [ebx + 0]
	__asm mov    al, [edx + eax]
	__asm mov    [ebx + 0], al
	__asm mov    al, [ebx + 1]
	__asm mov    al, [esi + eax]
	__asm mov    [ebx + 1], al
	__asm mov    al, [ebx + 2]
	__asm mov    al, [edi + eax]
	__asm mov    [ebx + 2], al
	__asm add    ebx, 3
	__asm loop   __3
__9:
	;
#else
	while(count >= 4)
	{
		p[0] = b[p[0]]; p[3] = b[p[3]]; p[6] = b[p[6]]; p[ 9] = b[p[ 9]];
		p[1] = g[p[1]]; p[4] = g[p[4]]; p[7] = g[p[7]]; p[10] = g[p[10]];
		p[2] = r[p[2]]; p[5] = r[p[5]]; p[8] = r[p[8]]; p[11] = r[p[11]];
		p += 12;
		count -= 4;
	}
	if(count & 2)
	{
		p[0] = b[p[0]]; p[3] = b[p[3]];
		p[1] = g[p[1]]; p[4] = g[p[4]];
		p[2] = r[p[2]]; p[5] = r[p[5]];
		p += 6;
	}
	if(count & 1)
	{
		p[0] = b[p[0]];
		p[1] = g[p[1]];
		p[2] = r[p[2]];
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void BltAlphaBlend24(byte *dest, const byte *src, unsigned count, unsigned alpha)
{
	if(alpha == 256)
		return;
	count *= 3;
	if(alpha == 0)
	{
		memcpy(dest, src, count);
		return;
	}
	int a = 256 - alpha;
	
//	__asm int 3
	
#ifdef I386_ASSEMBLY
	__int64 temp;
	
	if(!MMXDetect() || count < 32)
		goto __C;
	
	__asm mov esi, [src]
	__asm mov edi, [dest]
	__asm mov eax, [a]
	__asm mov ebx, eax
	__asm shl eax, 16
	__asm mov ax, bx
	__asm mov dword ptr [temp], eax
	__asm mov dword ptr [temp + 4], eax
	__asm pxor mm0, mm0
	__asm movq mm1, [temp]
	__asm mov ecx, [count]
	__asm mov eax, ecx
	__asm shr ecx, 5
	__asm je __norep
__rep:
#define BLT_ALPHA24_8(o) \
	__asm movq mm2, [esi + (o)] \
	__asm movq mm3, [edi + (o)] \
	__asm movq mm4, mm2 \
	__asm movq mm5, mm3 \
	__asm punpcklbw mm2, mm0 \
	__asm punpcklbw mm3, mm0 \
	__asm punpckhbw mm4, mm0 \
	__asm punpckhbw mm5, mm0 \
	__asm psubw mm2, mm3 \
	__asm psubw mm4, mm5 \
	__asm pmullw mm2, mm1 \
	__asm pmullw mm4, mm1 \
	__asm psrlw mm2, 8 \
	__asm psrlw mm4, 8 \
	__asm paddb mm2, mm3 \
	__asm paddb mm4, mm5 \
	__asm packuswb mm2, mm4 \
	__asm movq [edi + (o)], mm2 \
	
	BLT_ALPHA24_8(0)
	BLT_ALPHA24_8(8)
	BLT_ALPHA24_8(16)
	BLT_ALPHA24_8(24)

	__asm dec ecx
	__asm lea esi, [esi + 32]
	__asm lea edi, [edi + 32]
	__asm jne __rep
__norep:
	__asm test al, 16
	__asm je __no16
	BLT_ALPHA24_8(0)
	BLT_ALPHA24_8(8)
	__asm add esi, 16
	__asm add edi, 16
__no16:
	__asm test al, 8
	__asm je __no8
	BLT_ALPHA24_8(0)
	__asm add esi, 8
	__asm add edi, 8
__no8:
	__asm test al, 4
	__asm je __no4
	__asm movd mm2, [esi]
	__asm movd mm3, [edi]
	__asm punpcklbw mm2, mm0
	__asm punpcklbw mm3, mm0
	__asm psubw mm2, mm3
	__asm pmullw mm2, mm1
	__asm psrlw mm2, 8
	__asm paddb mm2, mm3
	__asm packuswb mm2, mm0
	__asm movd [edi], mm2
	__asm add esi, 4
	__asm add edi, 4
__no4:
	__asm emms
	__asm mov edx, [a]
	__asm test al, 2
	__asm je __no2
	__asm mov cl, [esi]
	__asm mov bl, [esi + 1]
	__asm sub cl, [edi]
	__asm sbb ch, ch
	__asm sub bl, [edi + 1]
	__asm sbb bh, bh
	__asm imul ecx, edx
	__asm imul ebx, edx
	__asm add [edi], ch
	__asm add [edi + 1], bh
	__asm add esi, 2
	__asm add edi, 2
__no2:
	__asm test al, 1
	__asm je __exit
	__asm mov cl, [esi]
	__asm sub cl, [edi]
	__asm sbb ch, ch
	__asm imul ecx, edx
	__asm add [edi], ch
__exit:
	return;

__C:
#endif

#define BLT_ALPHA24_1(o) dest[(o)] += ((src[(o)] - dest[(o)]) * a) >> 8;
#define BLT_ALPHA24_4(o) BLT_ALPHA24_1((o)) BLT_ALPHA24_1((o) + 1) BLT_ALPHA24_1((o) + 2) BLT_ALPHA24_1((o) + 3)

//	__asm int 3	
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_ALPHA24_4(0) BLT_ALPHA24_4(4) BLT_ALPHA24_4(8) BLT_ALPHA24_4(12)
		BLT_ALPHA24_4(16) BLT_ALPHA24_4(20) BLT_ALPHA24_4(24) BLT_ALPHA24_4(28)
		dest += 32;
		src += 32;
	}
	if(count & 16)
	{
		BLT_ALPHA24_4(0) BLT_ALPHA24_4(4) BLT_ALPHA24_4(8) BLT_ALPHA24_4(12)
		dest += 16;
		src += 16;
	}
	if(count & 8)
	{
		BLT_ALPHA24_4(0) BLT_ALPHA24_4(4)
		dest += 8;
		src += 8;
	}
	if(count & 4)
	{
		BLT_ALPHA24_4(0)
		dest += 4;
		src += 4;
	}
	if(count & 2)
	{
		BLT_ALPHA24_1(0) BLT_ALPHA24_1(1)
		dest += 2;
		src += 2;
	}
	if(count & 1)
	{
		BLT_ALPHA24_1(0)
	}
}

//////////////////////////////////////////////////////////////////////

void Blt2to1(byte *dest, const byte *src, unsigned count)
{
	byte a, b;
#define BLT2_1_8(o) \
		a = src[(o)] & 0x55, b = src[(o) + 1] & 0x55, a = (a << 1) | (a << 2); b = (b >> 1) | b; \
		dest[(o) / 2] = (a & 0xC0) | ((a << 2) & 0x30) | ((b >> 2) & 0x0C) | (b & 0x03);
		
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT2_1_8(0)
		BLT2_1_8(2)
		BLT2_1_8(4)
		BLT2_1_8(6)
		
		dest += 4;
		src += 8;
	}
	if(count & 16)
	{
		BLT2_1_8(0)
		BLT2_1_8(2)
		
		dest += 2;
		src += 4;
	}
	if(count & 8)
	{
		BLT2_1_8(0)
		
		dest++;
		src += 2;
	}
	if(count & 7)
	{
		byte out = 0, shift = 8;
		if(count & 4)
		{
			a = src[0] & 0x55;
			a = a | (a >> 1);
			out = (a & 0x03) | ((a >> 2) & 0xC);
			shift = 4;
			src++;
		}
		if(count & 3)
		{
			a = src[0];
			if(count & 2)
			{
				out = (out << 2) | ((a >> 5) & 2) | ((a >> 4) & 1);
				shift -= 2;
				a <<= 4;
			}
			if(count & 1)
			{
				out = (out << 1) | ((a >> 6) & 1);
				shift--;
			}
		}
		*dest = out << shift;
	}
}

//////////////////////////////////////////////////////////////////////

void Blt4to1(byte *dest, const byte *src, unsigned count)
{
#ifdef I386_ASSEMBLY
	__asm mov esi, [src]
	__asm mov edi, [dest]
	__asm mov eax, [count]
	__asm shr eax, 6
	__asm je __norep
__rep:
#define BLT4_1_8(o) \
	__asm mov cl, [esi + (o) + 0] \
	__asm mov ch, [esi + (o) + 1] \
	__asm mov dl, [esi + (o) + 2] \
	__asm mov dh, [esi + (o) + 3] \
	__asm and cl, 0x11 \
	__asm and ch, 0x11 \
	__asm and dl, 0x11 \
	__asm and dh, 0x11 \
	__asm shl cl, 2 \
	__asm shl dl, 2 \
	__asm or cl, ch \
	__asm or dl, dh \
	__asm mov ch, cl \
	__asm mov dh, dl \
	__asm shr ch, 3 \
	__asm shr dh, 3 \
	__asm or cl, ch \
	__asm or dl, dh \
	__asm shl cl, 4 \
	__asm and dl, 0x0F \
	__asm or cl, dl \
	__asm mov [edi + (o) / 4], cl

#define BLT4_1_32(o) BLT4_1_8((o) + 0) BLT4_1_8((o) + 4) BLT4_1_8((o) + 8) BLT4_1_8((o) + 12)

	BLT4_1_32(0)
	BLT4_1_32(16)
	
	__asm add esi, 32
	__asm add edi, 8
	__asm dec eax
	__asm jne __rep
__norep:
	__asm mov eax, [count]
//__no64:
	__asm test al, 32
	__asm je __no32
	
	BLT4_1_32(0)
	
	__asm add esi, 16
	__asm add edi, 4
__no32:
	__asm test al, 16
	__asm je __no16
	
	BLT4_1_8(0)
	BLT4_1_8(4)
	
	__asm add esi, 8
	__asm add edi, 2
__no16:
	__asm test al, 8
	__asm je __no8
	
	BLT4_1_8(0)

	__asm add esi, 4
	__asm inc edi
__no8:
	__asm test al, 7
	__asm je __exit
	__asm mov cl, 8
	__asm test al, 4
	__asm je __no4
	__asm mov cl, 4
	__asm mov dl, [esi + 0]
	__asm mov dh, [esi + 1]
	__asm and dl, 0x11
	__asm and dh, 0x11
	__asm shl dl, 2
	__asm or dl, dh
	__asm mov ch, dl
	__asm shr ch, 3
	__asm or ch, dl
	__asm add esi, 2
__no4:
	__asm test al, 2
	__asm je __no2
	__asm sub cl, 2
	__asm shl ch, 2
	__asm mov dl, [esi + 0]
	__asm mov dh, dl
	__asm and dh, 0x10
	__asm and dl, 1
	__asm shr dh, 3
	__asm or ch, dl
	__asm inc esi
	__asm or ch, dh
__no2:
	__asm test al, 1
	__asm je __no1
	__asm dec cl
	__asm mov dl, [esi]
	__asm shl dl, 4
	__asm rcl ch, 1
__no1:
	__asm shl ch, cl
	__asm mov [edi], ch
__exit:
	;
#else
	
	byte a, b;
#define BLT4_1_8(o) \
	a = ((src[(o) + 0] << 2) & 0x44) | (src[(o) + 1] & 0x11); \
	b = ((src[(o) + 2] << 2) & 0x44) | (src[(o) + 3] & 0x11); \
	dest[(o) / 4] = (a << 4) | ((a << 1) & 0xF0) | (b >> 3) | (b & 0xF);
	
	for(unsigned rep = count >> 6; rep; rep--)
	{
		BLT4_1_8(0) BLT4_1_8(4) BLT4_1_8(8) BLT4_1_8(12)
		BLT4_1_8(16) BLT4_1_8(20) BLT4_1_8(24) BLT4_1_8(28)
		dest += 8;
		src += 32;
	}
	if(count & 32)
	{
		BLT4_1_8(0) BLT4_1_8(4) BLT4_1_8(8) BLT4_1_8(12)
		dest += 4;
		src += 16;
	}
	if(count & 16)
	{
		BLT4_1_8(0) BLT4_1_8(4)
		dest += 2;
		src += 8;
	}
	if(count & 8)
	{
		BLT4_1_8(0)
		dest++;
		src += 4;
	}
	if(count & 7)
	{
		byte out = 0, shift = 8;
		if(count & 4)
		{
			a = ((src[0] << 2) & 0x44) | (src[1] & 0x11);
			out = (a & 0xF) | (a >> 3);
			src += 2;
			shift = 4;
		}
		if(count & 2)
		{
			out = (out << 2) | ((src[0] >> 3) & 2) | (src[0] & 1);
			src++;
			shift -= 2;
		}
		if(count & 1)
		{
			out = (out << 1) | ((src[0] >> 4) & 1);
			shift--;
		}
		*dest = out << shift;
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void Blt8to1(byte *dest, const byte *src, unsigned count)
{
	BLT_TIMING(Blt8to1)

#define BLT8_1_8(o) \
	dest[(o) / 8] = (src[(o)] << 7) | ((src[(o) + 1] << 6) & 0x40) | ((src[(o) + 2] << 5) & 0x20) | ((src[(o) + 3] << 4) & 0x10) \
	| ((src[(o) + 4] << 3) & 8) | ((src[(o) + 5] << 2) & 4) | ((src[(o) + 6] << 1) & 2) | (src[(o) + 7] & 1);

	for(unsigned rep = count >> 6; rep; rep--)
	{
		BLT8_1_8(0) BLT8_1_8(8) BLT8_1_8(16) BLT8_1_8(24)
		BLT8_1_8(32) BLT8_1_8(40) BLT8_1_8(48) BLT8_1_8(56)
		dest += 8;
		src += 64;
	}
	if(count & 32)
	{
		BLT8_1_8(0) BLT8_1_8(8) BLT8_1_8(16) BLT8_1_8(24)
		dest += 4;
		src += 32;
	}
	if(count & 16)
	{
		BLT8_1_8(0) BLT8_1_8(8)
		dest += 2;
		src += 16;
	}
	if(count & 8)
	{
		BLT8_1_8(0)
		dest++;
		src += 8;
	}
	switch(count & 7)
	{
	case 1: *dest = (src[0] << 7); break;
	case 2: *dest = (src[0] << 7) | ((src[1] << 6) & 0x40); break;
	case 3: *dest = (src[0] << 7) | ((src[1] << 6) & 0x40) | ((src[2] << 5) & 0x20); break;
	case 4: *dest = (src[0] << 7) | ((src[1] << 6) & 0x40) | ((src[2] << 5) & 0x20) | ((src[3] << 4) & 0x10); break;
	case 5: *dest = (src[0] << 7) | ((src[1] << 6) & 0x40) | ((src[2] << 5) & 0x20) | ((src[3] << 4) & 0x10) | ((src[4] << 3) & 0x08); break;
	case 6: *dest = (src[0] << 7) | ((src[1] << 6) & 0x40) | ((src[2] << 5) & 0x20) | ((src[3] << 4) & 0x10) | ((src[4] << 3) & 0x08) | ((src[5] << 2) & 0x04); break;
	case 7: *dest = (src[0] << 7) | ((src[1] << 6) & 0x40) | ((src[2] << 5) & 0x20) | ((src[3] << 4) & 0x10) | ((src[4] << 3) & 0x08) | ((src[5] << 2) & 0x04) | ((src[6] << 1) & 0x02); break;
	}
}

//////////////////////////////////////////////////////////////////////

void Blt1to4(byte *dest, const byte *src, unsigned count)
{
	BLT_TIMING(Blt1to4)
	
	byte a, b;
#define BLT1_4_8(o) \
	a = src[(o)]; \
	b = (a >> 4) | ((a >> 1) & 0xF0); \
	dest[4 * (o) + 0] = (b >> 2) & 0x11; \
	dest[4 * (o) + 1] = (b >> 0) & 0x11; \
	b = (a << 3) | (a & 7); \
	dest[4 * (o) + 2] = (b >> 2) & 0x11; \
	dest[4 * (o) + 3] = (b >> 0) & 0x11;

	for(unsigned rep = count >> 6; rep; rep--)
	{
		BLT1_4_8(0) BLT1_4_8(1) BLT1_4_8(2) BLT1_4_8(3)
		BLT1_4_8(4) BLT1_4_8(5) BLT1_4_8(6) BLT1_4_8(7)
		dest += 8 * 4;
		src += 8;
	}
	if(count & 32)
	{
		BLT1_4_8(0) BLT1_4_8(1) BLT1_4_8(2) BLT1_4_8(3)
		dest += 4 * 4;
		src += 4;
	}
	if(count & 16)
	{
		BLT1_4_8(0) BLT1_4_8(1)
		dest += 2 * 4;
		src += 2;
	}
	if(count & 8)
	{
		BLT1_4_8(0)
		dest += 1 * 4;
		src += 1;
	}
	if(count & 7)
	{
		a = src[0];
		if(count & 4)
		{
			b = (a >> 4) | ((a >> 1) & 0xF0); \
			dest[0] = (b >> 2) & 0x11; \
			dest[1] = (b >> 0) & 0x11; \
			dest += 2;
			a <<= 4;
		}
		if(count & 2)
		{
			*dest++ = ((a >> 3) & 0x10) | ((a >> 6) & 0x1);
			a <<= 2;
		}
		if(count & 1)
			*dest = ((a >> 3) & 0x10);
	}
}

//////////////////////////////////////////////////////////////////////

void Blt2to4(byte *dest, const byte *src, unsigned count)
{
	BLT_TIMING(Blt2to4)
	
	byte b;
	
#define BLT2_4_4(o) \
	b = src[(o)]; \
	dest[2 * (o) + 0] = ((b >> 2) & 0x30) | ((b >> 4) & 0x03); \
	dest[2 * (o) + 1] = ((b << 2) & 0x30) | (b & 0x03);
	
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT2_4_4(0) BLT2_4_4(1) BLT2_4_4(2) BLT2_4_4(3)
		BLT2_4_4(4) BLT2_4_4(5) BLT2_4_4(6) BLT2_4_4(7)
		dest += 8 * 2;
		src += 8;
	}
	if(count & 16)
	{
		BLT2_4_4(0) BLT2_4_4(1) BLT2_4_4(2) BLT2_4_4(3)
		dest += 4 * 2;
		src += 4;
	}
	if(count & 8)
	{
		BLT2_4_4(0) BLT2_4_4(1)
		dest += 2 * 2;
		src += 2;
	}
	if(count & 4)
	{
		BLT2_4_4(0)
		dest += 2;
		src++;
	}
	switch(count & 3)
	{
	case 0:
		break;
		
	case 1:
		*dest = ((*src >> 2) & 0x30);
		break;
		
	case 2:
		*dest = ((*src >> 2) & 0x30) | ((*src >> 4) & 0x03);
		break;
		
	case 3:
		*dest++ = ((*src >> 2) & 0x30) | ((*src >> 4) & 0x03);
		*dest = (*src << 2) & 0x30;
		break;
	}
}

//////////////////////////////////////////////////////////////////////

void Blt8to4(byte *dest, const byte *src, unsigned count)
{
	BLT_TIMING(Blt8to4)

#define BLT8_4_2(o) dest[(o) / 2] = (src[(o)] << 4) | (src[(o) + 1] & 15);
#define BLT8_4_8(o) BLT8_4_2((o)) BLT8_4_2((o) + 2) BLT8_4_2((o) + 4) BLT8_4_2((o) + 6)

	for(unsigned rep = count >> 6; rep; rep--)
	{
		BLT8_4_8(0) BLT8_4_8(8) BLT8_4_8(16) BLT8_4_8(24)
		BLT8_4_8(32) BLT8_4_8(40) BLT8_4_8(48) BLT8_4_8(56)
		dest += 32;
		src += 64;
	}
	if(count & 32)
	{
		BLT8_4_8(0) BLT8_4_8(8) BLT8_4_8(16) BLT8_4_8(24)
		dest += 16;
		src += 32;
	}
	if(count & 16)
	{
		BLT8_4_8(0) BLT8_4_8(8)
		dest += 8;
		src += 16;
	}
	if(count & 8)
	{
		BLT8_4_8(0)
		dest += 4;
		src += 8;
	}
	if(count & 4)
	{
		BLT8_4_2(0)
		BLT8_4_2(2)
		dest += 2;
		src += 4;
	}
	if(count & 2)
	{
		BLT8_4_2(0)
		dest++;
		src += 2;
	}
	if(count & 1)
		*dest = *src << 4;
}

//////////////////////////////////////////////////////////////////////

void Blt1to8(byte *dest, const byte *src, unsigned count)
{
	BLT_TIMING(Blt1to8)

	byte b;
	
#define BLT1_8_8(o) \
	b = src[o]; \
	dest[8 * (o) + 0] = (b >> 7); \
	dest[8 * (o) + 1] = (b >> 6) & 1; \
	dest[8 * (o) + 2] = (b >> 5) & 1; \
	dest[8 * (o) + 3] = (b >> 4) & 1; \
	dest[8 * (o) + 4] = (b >> 3) & 1; \
	dest[8 * (o) + 5] = (b >> 2) & 1; \
	dest[8 * (o) + 6] = (b >> 1) & 1; \
	dest[8 * (o) + 7] = (b >> 0) & 1; 

	for(unsigned rep = count >> 6; rep; rep--)
	{
		BLT1_8_8(0) BLT1_8_8(1) BLT1_8_8(2) BLT1_8_8(3)
		BLT1_8_8(4) BLT1_8_8(5) BLT1_8_8(6) BLT1_8_8(7)
		dest += 8 * 8;
		src += 8;
	}
	if(count & 32)
	{
		BLT1_8_8(0) BLT1_8_8(1) BLT1_8_8(2) BLT1_8_8(3)
		dest += 4 * 8;
		src += 4;
	}
	if(count & 16)
	{
		BLT1_8_8(0) BLT1_8_8(1)
		dest += 2 * 8;
		src += 2;
	}
	if(count & 8)
	{
		BLT1_8_8(0)
		dest += 8;
		src++;
	}
	if(count & 7)
	{
		byte b = *src;
		if(count & 4)
		{
			dest[0] = (b >> 7);
			dest[1] = (b >> 6) & 1;
			dest[2] = (b >> 5) & 1;
			dest[3] = (b >> 4) & 1;
			dest += 4;
			b <<= 4;
		}
		if(count & 2)
		{
			dest[0] = (b >> 7);
			dest[1] = (b >> 6) & 1;
			dest += 2;
			b <<= 2;
		}
		if(count & 1)
			*dest = b >> 7;
	}
}

//////////////////////////////////////////////////////////////////////

void Blt2to8(byte *dest, const byte *src, unsigned count)
{
	BLT_TIMING(Blt2to8)
	
	byte a;
	
#define BLT2_8_4(o) \
	a = src[(o)]; \
	dest[4 * (o) + 0] = (a >> 6); \
	dest[4 * (o) + 1] = (a >> 4) & 3; \
	dest[4 * (o) + 2] = (a >> 2) & 3; \
	dest[4 * (o) + 3] = (a >> 0) & 3;
	
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT2_8_4(0) BLT2_8_4(1) BLT2_8_4(2) BLT2_8_4(3)
		BLT2_8_4(4) BLT2_8_4(5) BLT2_8_4(6) BLT2_8_4(7)
		dest += 8 * 4;
		src += 8;
	}
	if(count & 16)
	{
		BLT2_8_4(0) BLT2_8_4(1) BLT2_8_4(2) BLT2_8_4(3)
		dest += 4 * 4;
		src += 4;
	}
	if(count & 8)
	{
		BLT2_8_4(0) BLT2_8_4(1)
		dest += 2 * 4;
		src += 2;
	}
	if(count & 4)
	{
		BLT2_8_4(0)
		dest += 4;
		src++;
	}
	if(count & 3)
	{
		a = src[0];
		if(count & 3)
		{
			dest[0] = (a >> 6) & 3;
			if(count & 2)
				dest[1] = (a >> 4) & 3;
			if(!(~count & 3))
				dest[2] = (a >> 2) & 3;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void Blt3to8(byte *dest, const byte *src, unsigned count)
{
#define BLT_38_8(o) \
	a = src[3 * (o) + 0]; b = src[3 * (o) + 1]; c = src[3 * (o) + 2]; \
	dest[8 * (o) + 0] = a >> 5; \
	dest[8 * (o) + 1] = (a >> 2) & 7; \
	dest[8 * (o) + 2] = ((a << 1) & 6) | (b >> 7); \
	dest[8 * (o) + 3] = (b >> 4) & 7; \
	dest[8 * (o) + 4] = (b >> 1) & 7; \
	dest[8 * (o) + 5] = ((b << 2) & 4) | (c >> 6); \
	dest[8 * (o) + 6] = (c >> 3) & 7; \
	dest[8 * (o) + 7] = c & 7;

	byte a, b, c;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_38_8(0) BLT_38_8(1) BLT_38_8(2) BLT_38_8(3)
		dest += 4 * 8;
		src += 4 * 3;
	}
	if(count & 16)
	{
		BLT_38_8(0) BLT_38_8(1)
		dest += 2 * 8;
		src += 2 * 3;
	}
	if(count & 8)
	{
		BLT_38_8(0)
		dest += 8;
		src += 3;
	}

	switch(count & 7)
	{
	case 7: dest[6] = (src[2] >> 3) & 7;
	case 6: dest[5] = ((src[1] << 2) & 4) | (src[2] >> 6);
	case 5: dest[4] = (src[1] >> 1) & 7;
	case 4: dest[3] = (src[1] >> 4) & 7;
	case 3: dest[2] = ((src[0] << 1) & 6) | (src[1] >> 7);
	case 2: dest[1] = (src[0] >> 2) & 7;
	case 1: dest[0] = src[0] >> 5;
	case 0: break;
	}
}

//////////////////////////////////////////////////////////////////////

void Blt4to8(byte *dest, const byte *src, unsigned count)
{
	BLT_TIMING(Blt4to8)
	
#define BLT4_8_2(o) dest[2 * (o) + 0] = src[(o)] >> 4; dest[2 * (o) + 1] = src[(o)] & 15;
#define BLT4_8_8(o) BLT4_8_2((o)) BLT4_8_2((o) + 1) BLT4_8_2((o) + 2) BLT4_8_2((o) + 3)
	
	for(unsigned rep = count >> 6; rep; rep--)
	{
		BLT4_8_8(0) BLT4_8_8(4) BLT4_8_8(8) BLT4_8_8(12)
		BLT4_8_8(16) BLT4_8_8(20) BLT4_8_8(24) BLT4_8_8(28)
		dest += 64;
		src += 32;
	}
	if(count & 32)
	{
		BLT4_8_8(0) BLT4_8_8(4) BLT4_8_8(8) BLT4_8_8(12)
		dest += 32;
		src += 16;
	}
	if(count & 16)
	{
		BLT4_8_8(0) BLT4_8_8(4)
		dest += 16;
		src += 8;
	}
	if(count & 8)
	{
		BLT4_8_8(0)
		dest += 8;
		src += 4;
	}
	if(count & 4)
	{
		BLT4_8_2(0) BLT4_8_2(1)
		dest += 4;
		src += 2;
	}
	if(count & 2)
	{
		BLT4_8_2(0)
		dest += 2;
		src++;
	}
	if(count & 1)
		*dest = *src >> 4;
}

//////////////////////////////////////////////////////////////////////

void Blt5to8(byte *dest, const byte *src, unsigned count)
{
#define BLT_58_8(o) \
	dest[8 * (o) + 0] = src[5 * (o) + 0] >> 3; \
	dest[8 * (o) + 1] = ((src[5 * (o) + 0] << 2) & 0x1C) | (src[5 * (o) + 1] >> 6); \
	dest[8 * (o) + 2] = ((src[5 * (o) + 1] >> 1) & 0x1F); \
	dest[8 * (o) + 3] = ((src[5 * (o) + 1] << 4) & 0x10) | (src[5 * (o) + 2] >> 4); \
	dest[8 * (o) + 4] = ((src[5 * (o) + 2] << 1) & 0x1E) | (src[5 * (o) + 3] >> 7); \
	dest[8 * (o) + 5] = ((src[5 * (o) + 3] >> 2) & 0x1F); \
	dest[8 * (o) + 6] = ((src[5 * (o) + 3] << 3) & 0x18) | (src[5 * (o) + 4] >> 5); \
	dest[8 * (o) + 7] = src[5 * (o) + 4] & 0x1F;

	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_58_8(0) BLT_58_8(1) BLT_58_8(2) BLT_58_8(3)
		dest += 4 * 8;
		src += 4 * 5;
	}
	if(count & 16)
	{
		BLT_58_8(0) BLT_58_8(1)
		dest += 2 * 8;
		src += 2 * 5;
	}
	if(count & 8)
	{
		BLT_58_8(0)
		dest += 8;
		src += 5;
	}
	switch(count & 7)
	{
	case 7: dest[6] = ((src[3] << 3) & 0x18) | (src[4] >> 5);
	case 6: dest[5] = ((src[3] >> 2) & 0x1F);
	case 5: dest[4] = ((src[2] << 1) & 0x1E) | (src[3] >> 7);
	case 4: dest[3] = ((src[1] << 4) & 0x10) | (src[2] >> 4);
	case 3: dest[2] = ((src[1] >> 1) & 0x1F);
	case 2: dest[1] = ((src[0] << 2) & 0x1C) | (src[1] >> 6);
	case 1: dest[0] = src[0] >> 3;
	case 0: break;
	}
}

//////////////////////////////////////////////////////////////////////

void Blt6to8(byte *dest, const byte *src, unsigned count)
{
#define BLT_68_4(o) \
	dest[4 * (o) + 0] = src[3 * (o) + 0] >> 2; \
	dest[4 * (o) + 1] = ((src[3 * (o) + 0] << 4) & 0x30) | (src[3 * (o) + 1] >> 4); \
	dest[4 * (o) + 2] = ((src[3 * (o) + 1] << 2) & 0x3C) | (src[3 * (o) + 2] >> 6); \
	dest[4 * (o) + 3] = src[3 * (o) + 2] & 0x3F;

	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_68_4(0) BLT_68_4(1) BLT_68_4(2) BLT_68_4(3)
		BLT_68_4(4) BLT_68_4(5) BLT_68_4(6) BLT_68_4(7)
		dest += 8 * 4;
		src += 8 * 3;
	}
	if(count & 16)
	{
		BLT_68_4(0) BLT_68_4(1) BLT_68_4(2) BLT_68_4(3)
		dest += 4 * 4;
		src += 4 * 3;
	}
	if(count & 8)
	{
		BLT_68_4(0) BLT_68_4(1)
		dest += 2 * 4;
		src += 2 * 3;
	}
	if(count & 4)
	{
		BLT_68_4(0)
		dest += 4;
		src += 3;
	}
	switch(count & 3)
	{
	case 3: dest[2] = ((src[1] << 2) & 0x3C) | (src[2] >> 6);
	case 2: dest[1] = ((src[0] << 4) & 0x30) | (src[1] >> 4);
	case 1: dest[0] = src[0] >> 2;
	case 0: break;
	}
}

//////////////////////////////////////////////////////////////////////

void Blt7to8(byte *dest, const byte *src, unsigned count)
{
#define BLT_78_8(o) \
	dest[8 * (o) + 0] = src[7 * (o) + 0] >> 1; \
	dest[8 * (o) + 1] = ((src[7 * (o) + 0] << 6) & 0x40) | (src[7 * (o) + 1] >> 2); \
	dest[8 * (o) + 2] = ((src[7 * (o) + 1] << 5) & 0x60) | (src[7 * (o) + 2] >> 3); \
	dest[8 * (o) + 3] = ((src[7 * (o) + 2] << 4) & 0x70) | (src[7 * (o) + 3] >> 4); \
	dest[8 * (o) + 4] = ((src[7 * (o) + 3] << 3) & 0x78) | (src[7 * (o) + 4] >> 5); \
	dest[8 * (o) + 5] = ((src[7 * (o) + 4] << 2) & 0x7C) | (src[7 * (o) + 5] >> 6); \
	dest[8 * (o) + 6] = ((src[7 * (o) + 5] << 1) & 0x7E) | (src[7 * (o) + 6] >> 7); \
	dest[8 * (o) + 7] = ((src[7 * (o) + 6] << 0) & 0x7F);

	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_78_8(0) BLT_78_8(1) BLT_78_8(2) BLT_78_8(3)
		dest += 4 * 8;
		src += 4 * 7;
	}
	if(count & 16)
	{
		BLT_78_8(0) BLT_78_8(1)
		dest += 2 * 8;
		src += 2 * 7;
	}
	if(count & 8)
	{
		BLT_78_8(0)
		dest += 8;
		src += 7;
	}
	switch(count & 7)
	{
	case 7: dest[6] = ((src[5] << 1) & 0x7E) | (src[6] >> 7);
	case 6: dest[5] = ((src[4] << 2) & 0x7C) | (src[5] >> 6);
	case 5: dest[4] = ((src[3] << 3) & 0x78) | (src[4] >> 5);
	case 4: dest[3] = ((src[2] << 4) & 0x70) | (src[3] >> 4);
	case 3: dest[2] = ((src[1] << 5) & 0x60) | (src[2] >> 3);
	case 2: dest[1] = ((src[0] << 6) & 0x40) | (src[1] >> 2);
	case 1: dest[0] = src[0] >> 1;
	case 0: break;
	}
}

//////////////////////////////////////////////////////////////////////

void BltMask8(byte *dest, const byte *mask, unsigned count, byte mask_value)
{
	while(count >= 8)
	{
		if(mask[0]) dest[0] = mask_value;
		if(mask[1]) dest[1] = mask_value;
		if(mask[2]) dest[2] = mask_value;
		if(mask[3]) dest[3] = mask_value;
		if(mask[4]) dest[4] = mask_value;
		if(mask[5]) dest[5] = mask_value;
		if(mask[6]) dest[6] = mask_value;
		if(mask[7]) dest[7] = mask_value;
		mask += 8;
		dest += 8;
		count -= 8;
	}
	if(count & 4)
	{
		if(mask[0]) dest[0] = mask_value;
		if(mask[1]) dest[1] = mask_value;
		if(mask[2]) dest[2] = mask_value;
		if(mask[3]) dest[3] = mask_value;
		mask += 4;
		dest += 4;
	}
	if(count & 2)
	{
		if(mask[0]) dest[0] = mask_value;
		if(mask[1]) dest[1] = mask_value;
		mask += 2;
		dest += 2;
	}
	if(count & 1)
		if(mask[0]) dest[0] = mask_value;
}

//////////////////////////////////////////////////////////////////////

void BltBuildMaskRgb(byte *dest, const byte *src, unsigned count, const byte rRgGbB[6])
{
#ifdef I386_ASSEMBLY
	__asm mov    edi, [dest]
	__asm mov    esi, [src]
	__asm mov    edx, [rRgGbB]
	__asm mov    ax, [edx + 0]
	__asm mov    bx, [edx + 2]
	__asm mov    cx, [edx + 4]
	__asm sub    ah, al
	__asm sub    bh, bl
	__asm sub    ch, cl
	__asm push   ebp
	__asm mov    ebp, [count]
	__asm shr    ebp, 1
	__asm je     __1
__2:
	__asm mov    dx, [esi]
	__asm sub    dl, cl
	__asm sub    dh, bl
	__asm cmp    dl, ch
	__asm setbe  dl
	__asm cmp    dh, dh
	__asm setbe  dh
	__asm and    dh, dl
	__asm mov    dl, [esi + 2]
	__asm sub    dl, al
	__asm cmp    dl, ah
	__asm setbe  dl
	__asm and    dh, dl
	__asm and    dh, 1
	__asm mov    [edi], dh
	__asm mov    dx, [esi + 4]
	__asm sub    dl, bl
	__asm sub    dh, al
	__asm cmp    dl, bh
	__asm setbe  dl
	__asm cmp    dh, ah
	__asm setbe  dh
	__asm and    dh, dl
	__asm mov    dl, [esi + 3]
	__asm sub    dl, cl
	__asm cmp    dl, ch
	__asm setbe  dl
	__asm and    dh, dl
	__asm and    dh, 1
	__asm mov    [edi + 1], dh
	__asm add    esi, 6
	__asm add    edi, 2
	__asm dec    ebp
	__asm jne    __2
__1:
	__asm pop    ebp
	__asm test   [count], 1
	__asm je     __3
	__asm mov    dx, [esi]
	__asm sub    dl, cl
	__asm sub    dh, bl
	__asm cmp    dl, ch
	__asm setbe  dl
	__asm cmp    dh, bh
	__asm setbe  dh
	__asm and    dh, dl
	__asm mov    dl, [esi + 2]
	__asm sub    dl, al
	__asm cmp    dl, ah
	__asm setbe  dl
	__asm and    dh, dl
	__asm and    dh, 1
	__asm mov    [edi], dh
__3:
	;
#else
	register byte r = rRgGbB[0], cr = rRgGbB[1] - r;
	register byte g = rRgGbB[2], cg = rRgGbB[3] - g;
	register byte b = rRgGbB[4], cb = rRgGbB[5] - b;

#define PIXEL(i) \
	dest[i] = ((byte)(src[3 * i + 2] - r) <= cr \
		&&     (byte)(src[3 * i + 1] - g) <= cg \
		&&     (byte)(src[3 * i + 0] - b) <= cb ? 1 : 0)

	while(count >= 4)
	{
		PIXEL(0);
		PIXEL(1);
		PIXEL(2);
		PIXEL(3);
		dest += 4;
		src += 12;
		count -= 4;
	}
	
	if(count & 2)
	{
		PIXEL(0);
		PIXEL(1);
		dest += 2;
		src += 6;
	}

	if(count & 1)
		PIXEL(0);
#endif
}

//////////////////////////////////////////////////////////////////////

void BltKillMask1(byte *dest, const byte *src, unsigned count)
{
#define BLT_KILL_MASK1_2(o) a = src[(o)] - 1; b = src[(o) + 1] - 1; dest[(o)] &= a; dest[(o) + 1] &= b;
#define BLT_KILL_MASK1_8(o) BLT_KILL_MASK1_2((o)) BLT_KILL_MASK1_2((o) + 2) BLT_KILL_MASK1_2((o) + 4) BLT_KILL_MASK1_2((o) + 6)
	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_KILL_MASK1_8(0) BLT_KILL_MASK1_8(8) BLT_KILL_MASK1_8(16) BLT_KILL_MASK1_8(24)
		dest += 32;
		src += 32;
	}
	if(count & 16)
	{
		BLT_KILL_MASK1_8(0) BLT_KILL_MASK1_8(8)
		dest += 16;
		src += 16;
	}
	if(count & 8)
	{
		BLT_KILL_MASK1_8(0)
		dest += 8;
		src += 8;
	}
	if(count & 4)
	{
		BLT_KILL_MASK1_2(0) BLT_KILL_MASK1_2(2)
		dest += 4;
		src += 4;
	}
	if(count & 2)
	{
		BLT_KILL_MASK1_2(0)
		dest += 2;
		src += 2;
	}
	if(count & 1)
		dest[0] &= -src[0];
}

//////////////////////////////////////////////////////////////////////

void BltKillMask3(byte *dest, const byte *src, unsigned count)
{
#define BLT_KILL_MASK3_2(o) a = src[(o)] - 1; b = src[(o) + 1] - 1; dest[3 * (o) + 0] &= a; dest[3 * (o) + 1] &= a; dest[3 * (o) + 2] &= a; dest[3 * (o) + 3] &= b; dest[3 * (o) + 4] &= b; dest[3 * (o) + 5] &= b;
#define BLT_KILL_MASK3_8(o) BLT_KILL_MASK3_2((o)) BLT_KILL_MASK3_2((o) + 2) BLT_KILL_MASK3_2((o) + 4) BLT_KILL_MASK3_2((o) + 6)
	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_KILL_MASK3_8(0) BLT_KILL_MASK3_8(8) BLT_KILL_MASK3_8(16) BLT_KILL_MASK3_8(24)
		dest += 3 * 32;
		src += 32;
	}
	if(count & 16)
	{
		BLT_KILL_MASK3_8(0) BLT_KILL_MASK3_8(8)
		dest += 3 * 16;
		src += 16;
	}
	if(count & 8)
	{
		BLT_KILL_MASK3_8(0)
		dest += 3 * 8;
		src += 8;
	}
	if(count & 4)
	{
		BLT_KILL_MASK3_2(0) BLT_KILL_MASK3_2(2)
		dest += 3 * 4;
		src += 4;
	}
	if(count & 2)
	{
		BLT_KILL_MASK3_2(0)
		dest += 3 * 2;
		src += 2;
	}
	if(count & 1)
		dest[0] &= -src[0];
}

//////////////////////////////////////////////////////////////////////

void BltRopCopy(byte *dest, const byte *src, unsigned count)
{
	memcpy(dest, src, count);
}

//////////////////////////////////////////////////////////////////////

void BltRopAnd(byte *dest, const byte *src, unsigned count)
{
#ifdef I386_ASSEMBLY
	__asm mov edi, [dest]
	__asm mov esi, [src]
	__asm mov ecx, [count]
	__asm shr ecx, 4
	__asm jcxz __1
__2:
	__asm mov ebx, [esi]
	__asm mov edx, [esi + 4]
	__asm and [edi], ebx
	__asm and [edi + 4], edx
	__asm mov ebx, [esi + 8]
	__asm mov edx, [esi + 12]
	__asm and [edi + 8], ebx
	__asm and [edi + 12], edx
	__asm add edi, 16
	__asm add esi, 16
	__asm loop __2
__1:
	__asm test [count], 8
	__asm je __3
	__asm mov ebx, [esi]
	__asm mov edx, [esi + 4]
	__asm and [edi], ebx
	__asm and [edi + 4], edx
	__asm add edi, 8
	__asm add esi, 8
__3:
	__asm test [count], 4
	__asm je __4
	__asm mov ebx, [esi]
	__asm and [edi], ebx
	__asm add edi, 4
	__asm add esi, 4
__4:
	__asm test [count], 2
	__asm je __5
	__asm mov bx, [esi]
	__asm and [edi], bx
	__asm add edi, 2
	__asm add esi, 2
__5:
	__asm test [count], 1
	__asm je __6
	__asm mov bl, [esi]
	__asm and [edi], bl
__6:
	;
#else
	while(count >= 4)
	{
		dest[0] &= src[0]; dest[1] &= src[1]; dest[2] &= src[2]; dest[3] &= src[3];
		dest += 4;
		src += 4;
		count -= 4;
	}
	if(count & 2)
	{
		dest[0] &= src[0]; dest[1] &= src[1];
		dest += 2;
		src += 2;
	}
	if(count & 1)
		dest[0] &= src[0];
#endif
}

//////////////////////////////////////////////////////////////////////

void BltRopXor(byte *dest, const byte *src, unsigned count)
{
#ifdef I386_ASSEMBLY
	__asm mov edi, [dest]
	__asm mov esi, [src]
	__asm mov ecx, [count]
	__asm shr ecx, 4
	__asm jcxz __1
__2:
	__asm mov ebx, [esi]
	__asm mov edx, [esi + 4]
	__asm xor [edi], ebx
	__asm xor [edi + 4], edx
	__asm mov ebx, [esi + 8]
	__asm mov edx, [esi + 12]
	__asm xor [edi + 8], ebx
	__asm xor [edi + 12], edx
	__asm add edi, 16
	__asm add esi, 16
	__asm loop __2
__1:
	__asm test [count], 8
	__asm je __3
	__asm mov ebx, [esi]
	__asm mov edx, [esi + 4]
	__asm xor [edi], ebx
	__asm xor [edi + 4], edx
	__asm add edi, 8
	__asm add esi, 8
__3:
	__asm test [count], 4
	__asm je __4
	__asm mov ebx, [esi]
	__asm xor [edi], ebx
	__asm add edi, 4
	__asm add esi, 4
__4:
	__asm test [count], 2
	__asm je __5
	__asm mov bx, [esi]
	__asm xor [edi], bx
	__asm add edi, 2
	__asm add esi, 2
__5:
	__asm test [count], 1
	__asm je __6
	__asm mov bl, [esi]
	__asm xor [edi], bl
__6:
	;
#else
	while(count >= 4)
	{
		dest[0] ^= src[0]; dest[1] ^= src[1]; dest[2] ^= src[2]; dest[3] ^= src[3];
		dest += 4;
		src += 4;
		count -= 4;
	}
	if(count & 2)
	{
		dest[0] ^= src[0]; dest[1] ^= src[1];
		dest += 2;
		src += 2;
	}
	if(count & 1)
		dest[0] ^= src[0];
#endif
}

//////////////////////////////////////////////////////////////////////

#define BLT_SPLIT_RGBA_C2(o) \
	a = src[4 * (o) + 0]; b = src[4 * (o) + 1]; \
	bgr[3 * (o) + 2] = a; bgr[3 * (o) + 1] = b; \
	a = src[4 * (o) + 2]; b = src[4 * (o) + 4]; \
	bgr[3 * (o) + 0] = a; bgr[3 * (o) + 5] = b; \
	a = src[4 * (o) + 5]; b = src[4 * (o) + 6]; \
	bgr[3 * (o) + 4] = a; bgr[3 * (o) + 3] = b;
	
#define BLT_SPLIT_RGBA_C8(o) \
	BLT_SPLIT_RGBA_C2((o) + 0) BLT_SPLIT_RGBA_C2((o) + 2) \
	BLT_SPLIT_RGBA_C2((o) + 4) BLT_SPLIT_RGBA_C2((o) + 6)
	
#define BLT_SPLIT_RGBA_8(o) \
	BLT_SPLIT_RGBA_C8(o) \
	mask[(o)] = \
	((src[4 * (o) +  3] >> 0) & 0x80) | \
	((src[4 * (o) +  7] >> 1) & 0x40) | \
	((src[4 * (o) + 11] >> 2) & 0x20) | \
	((src[4 * (o) + 15] >> 3) & 0x10) | \
	((src[4 * (o) + 19] >> 4) & 0x08) | \
	((src[4 * (o) + 23] >> 5) & 0x04) | \
	((src[4 * (o) + 27] >> 6) & 0x02) | \
	((src[4 * (o) + 31] >> 7)); \
	
void BltSwapRGBA(byte *bgr, byte *mask, const byte *src, unsigned count)
{
	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_SPLIT_RGBA_8(0)
		BLT_SPLIT_RGBA_8(8)
		BLT_SPLIT_RGBA_8(16)
		BLT_SPLIT_RGBA_8(24)
		src += 4 * 32;
		bgr += 3 * 32;
		mask += 4;
	}
	if(count & 16)
	{
		BLT_SPLIT_RGBA_8(0)
		BLT_SPLIT_RGBA_8(8)
		src += 4 * 16;
		bgr += 3 * 16;
		mask += 2;
	}
	if(count & 8)
	{
		BLT_SPLIT_RGBA_8(0)
		src += 4 * 8;
		bgr += 3 * 8;
		mask++;
	}
	if(count & 7)
	{
		byte mterm = 0, shift = 8;
		if(count & 4)
		{
			BLT_SPLIT_RGBA_C2(0)
			BLT_SPLIT_RGBA_C2(2)
			shift = 4;
			mterm = ((src[3] >> 4) & 8) | ((src[7] >> 5) & 4) | ((src[11] >> 6) & 2) | (src[15] >> 7);
			src += 16;
			bgr += 12;
		}
		if(count & 2)
		{
			BLT_SPLIT_RGBA_C2(0)
			shift -= 2;
			mterm = (mterm << 2) | ((src[3] >> 6) & 2) | ((src[7] >> 7) & 1);
			src += 8;
			bgr += 6;
		}
		if(count & 1)
		{
			bgr[0] = src[2]; bgr[1] = src[1]; bgr[2] = src[0];
			shift--;
			mterm = (mterm << 1) | (src[3] >> 7);
		}
		*mask = mterm << shift;
	}
}

//////////////////////////////////////////////////////////////////////

void BltSwapRGB4(byte *bgr, const byte *src, unsigned count)
{
//	Break();
	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_SPLIT_RGBA_C8(0) BLT_SPLIT_RGBA_C8(8) BLT_SPLIT_RGBA_C8(16) BLT_SPLIT_RGBA_C8(24)
		src += 4 * 32;
		bgr += 3 * 32;
	}
	if(count & 16)
	{
		BLT_SPLIT_RGBA_C8(0) BLT_SPLIT_RGBA_C8(8)
		src += 4 * 16;
		bgr += 3 * 16;
	}
	if(count & 8)
	{
		BLT_SPLIT_RGBA_C8(0)
		src += 4 * 8;
		bgr += 3 * 8;
	}
	if(count & 4)
	{
		BLT_SPLIT_RGBA_C2(0) BLT_SPLIT_RGBA_C2(2)
		src += 16;
		bgr += 12;
	}
	if(count & 2)
	{
		BLT_SPLIT_RGBA_C2(0)
		src += 8;
		bgr += 6;
	}
	if(count & 1)
	{
		bgr[0] = src[2]; bgr[1] = src[1]; bgr[2] = src[0];
	}
}

//////////////////////////////////////////////////////////////////////

void BltExpand3(byte *dest, const byte *src, unsigned count)
{
#define BLT_EXPAND3_2(o) \
	a = src[(o) + 0]; b = src[(o) + 1]; \
	dest[3 * (o) + 0] = a; dest[3 * (o) + 3] = b;

#define BLT_EXPAND3_8(o) \
	BLT_EXPAND3_2((o) + 0) BLT_EXPAND3_2((o) + 2) \
	BLT_EXPAND3_2((o) + 4) BLT_EXPAND3_2((o) + 6) \

	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_EXPAND3_8(0) BLT_EXPAND3_8(8)
		BLT_EXPAND3_8(16) BLT_EXPAND3_8(24)
		src += 32;
		dest += 3 * 32;
	}
	if(count & 16)
	{
		BLT_EXPAND3_8(0) BLT_EXPAND3_8(8)
		src += 16;
		dest += 3 * 16;
	}
	if(count & 8)
	{
		BLT_EXPAND3_8(0)
		src += 8;
		dest += 3 * 8;
	}
	if(count & 4)
	{
		BLT_EXPAND3_2(0) BLT_EXPAND3_2(2)
		src += 4;
		dest += 3 * 4;
	}
	if(count & 2)
	{
		BLT_EXPAND3_2(0)
		src += 2;
		dest += 3 * 2;
	}
	if(count & 1)
		dest[0] = src[0];
}

//////////////////////////////////////////////////////////////////////

void BltExpand4(byte *dest, const byte *src, unsigned count)
{
#define BLT_EXPAND4_2(o) \
	a = src[(o) + 0]; b = src[(o) + 1]; \
	dest[4 * (o) + 0] = a; dest[4 * (o) + 4] = b;

#define BLT_EXPAND4_8(o) \
	BLT_EXPAND4_2((o) + 0) BLT_EXPAND4_2((o) + 2) \
	BLT_EXPAND4_2((o) + 4) BLT_EXPAND4_2((o) + 6) \

	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_EXPAND4_8( 0) BLT_EXPAND4_8( 8)
		BLT_EXPAND4_8(16) BLT_EXPAND4_8(24)
		src += 32;
		dest += 4 * 32;
	}
	if(count & 16)
	{
		BLT_EXPAND4_8(0) BLT_EXPAND4_8(8)
		src += 16;
		dest += 4 * 16;
	}
	if(count & 8)
	{
		BLT_EXPAND4_8(0)
		src += 8;
		dest += 4 * 8;
	}
	if(count & 4)
	{
		BLT_EXPAND4_2(0) BLT_EXPAND4_2(2)
		src += 4;
		dest += 4 * 4;
	}
	if(count & 2)
	{
		BLT_EXPAND4_2(0)
		src += 2;
		dest += 4 * 2;
	}
	if(count & 1)
		dest[0] = src[0];
}

//////////////////////////////////////////////////////////////////////

void BltInvert(byte *dest, unsigned count)
{
#define BLT_INVERT_8(o) *(int *)(dest + (o) + 0) ^= -1; *(int *)(dest + (o) + 4) ^= -1;

	ASSERT(sizeof(int) == 4);
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_INVERT_8(0) BLT_INVERT_8(8) BLT_INVERT_8(16) BLT_INVERT_8(24)
		dest += 32;
	}
	if(count & 16)
	{
		BLT_INVERT_8(0) BLT_INVERT_8(8)
		dest += 16;
	}
	if(count & 8)
	{
		BLT_INVERT_8(0)
		dest += 8;
	}
	if(count & 4)
	{
		*(int *)dest ^= -1;
		dest += 4;
	}
	if(count & 2)
	{
		dest[0] ^= -1;
		dest[1] ^= -1;
		dest += 2;
	}
	if(count & 1)
		*dest ^= -1;
}

//////////////////////////////////////////////////////////////////////
#endif
