//////////////////////////////////////////////////////////////////////
// jpeg_add.c: additional routines for JPEG processor.

#include "lib/jinclude.h"
#include "lib/jpeglib.h"
#include <assert.h>

#ifdef COMPILER_MSC

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////

void BeginTiming();
void EndTiming();

typedef unsigned char byte;

//////////////////////////////////////////////////////////////////////

/*
static int IsMMXAvail()
{__asm{
	mov       eax, 1
	cpuid
	shl       edx, 9
	sbb       eax, eax
	neg       eax
}}
*/

//////////////////////////////////////////////////////////////////////

static void Upsample22Row(const byte *src0, const byte *src1, byte *dest, int items)
{__asm{
	mov       esi, [src0]
	mov       ebx, [src1]
	mov       edi, [dest]

	xor       eax, eax
	xor       edx, edx
	mov       al, [esi]
	mov       dl, [ebx]
	add       edx, eax
	lea       edx, [2 * eax + edx]
	lea       eax, [edx + 2]
	shr       eax, 2
	xor       ecx, ecx
	mov       [edi], al
	mov       cl, [ebx + 1]
	mov       al, [esi + 1] ; high 24 bits of eax are still zero (shr eax, 2)
	add       ecx, eax
	add       ebx, 2
	lea       ecx, [2 * eax + ecx]
	lea       eax, [2 * edx + ecx + 7]
	add       eax, edx
	add       esi, 2
	shr       eax, 4
	mov       [edi + 1], al

	push      ebp
	mov       ebp, [items]
	dec       ebp
	je        __9
	dec       ebp
	je        __2
	sub       ebp, 2
	jb        __3
__1:
	lea       eax, [2 * ecx + edx + 8]
	add       edi, 4
	add       eax, ecx
	shr       eax, 4
	xor       edx, edx
	mov       [edi - 2], al
	mov       dl, [ebx]
	mov       al, [esi]
	add       edx, eax
	lea       edx, [2 * eax + edx]
	lea       eax, [2 * ecx + edx + 7]
	add       eax, ecx
	shr       eax, 4
	mov       [edi - 1], al

	lea       eax, [2 * edx + ecx + 8]
	add       eax, edx
	shr       eax, 4
	xor       ecx, ecx
	mov       [edi], al
	mov       cl, [ebx + 1]
	mov       al, [esi + 1]
	add       ebx, 2
	add       ecx, eax
	lea       ecx, [2 * eax + ecx]
	lea       eax, [2 * edx + ecx + 7]
	add       esi, 2
	add       eax, edx
	shr       eax, 4
	mov       [edi + 1], al

	sub       ebp, 2
	jnb       __1
__3:
	shr       ebp, 1
	jnc       __2

	lea       eax, [2 * ecx + edx + 8]
	add       eax, ecx
	shr       eax, 4
	xor       edx, edx
	mov       [edi + 2], al
	mov       dl, [ebx]
	mov       al, [esi]
	inc       ebx
	add       edx, eax
	lea       edx, [2 * eax + edx]
	lea       eax, [2 * ecx + edx + 7]
	inc       esi
	add       eax, ecx
	shr       eax, 4
	mov       [edi + 3], al
	xchg      ecx, edx
	add       edi, 2
__2:
	pop       ebp
	lea       eax, [2 * ecx + edx + 8]
	add       eax, ecx
	shr       eax, 4
	inc       ecx
	mov       [edi + 2], al
	shr       ecx, 2
	mov       [edi + 3], cl
__9:
}}

//////////////////////////////////////////////////////////////////////

void
h2v2_fancy_upsample (j_decompress_ptr cinfo, jpeg_component_info * compptr,
JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr)
{
/*
	JSAMPARRAY output_data = *output_data_ptr;
	register JSAMPROW inptr0, inptr1, outptr;
	register int thiscolsum, lastcolsum, nextcolsum;
	register JDIMENSION colctr;
	int inrow, outrow, v;
//*/
//	BeginTiming();

	assert(BITS_IN_JSAMPLE == 8);

	{
		JSAMPARRAY output_data = *output_data_ptr;
		int inrow = 0, outrow = 0;

		while(outrow < cinfo -> max_v_samp_factor)
		{
			Upsample22Row(input_data[inrow], input_data[inrow - 1], output_data[outrow++], compptr -> downsampled_width);
			Upsample22Row(input_data[inrow], input_data[inrow + 1], output_data[outrow++], compptr -> downsampled_width);
			inrow++;
		}

	}

//	EndTiming();
/*
	inrow = outrow = 0;
	while (outrow < cinfo->max_v_samp_factor)
	{
		for (v = 0; v < 2; v++)
		{
			// inptr0 points to nearest input row, inptr1 points to next nearest
			inptr0 = input_data[inrow];
			if (v == 0)		// next nearest is row above
				inptr1 = input_data[inrow-1];
			else			// next nearest is row below
				inptr1 = input_data[inrow+1];
			outptr = output_data[outrow++];

			// Special case for first column
			thiscolsum = GETJSAMPLE(*inptr0++) * 3 + GETJSAMPLE(*inptr1++);
			nextcolsum = GETJSAMPLE(*inptr0++) * 3 + GETJSAMPLE(*inptr1++);
			assert(*outptr++ == (JSAMPLE) ((thiscolsum * 4 + 8) >> 4));
			assert(*outptr++ == (JSAMPLE) ((thiscolsum * 3 + nextcolsum + 7) >> 4));
			lastcolsum = thiscolsum; thiscolsum = nextcolsum;

			for (colctr = compptr->downsampled_width - 2; colctr > 0; colctr--)
			{
				// General case: 3/4 * nearer pixel + 1/4 * further pixel in each
				// dimension, thus 9/16, 3/16, 3/16, 1/16 overall
				nextcolsum = GETJSAMPLE(*inptr0++) * 3 + GETJSAMPLE(*inptr1++);
				assert(*outptr++ == (JSAMPLE) ((thiscolsum * 3 + lastcolsum + 8) >> 4));
				assert(*outptr++ == (JSAMPLE) ((thiscolsum * 3 + nextcolsum + 7) >> 4));
				lastcolsum = thiscolsum; thiscolsum = nextcolsum;
			}

			// Special case for last column
			assert(*outptr++ == (JSAMPLE) ((thiscolsum * 3 + lastcolsum + 8) >> 4));
			assert(*outptr++ == (JSAMPLE) ((thiscolsum * 4 + 7) >> 4));
		}
		inrow++;
	}
//*/
}

//////////////////////////////////////////////////////////////////////

void YccRgbConvertRow(const byte *y, const byte *cb, const byte *cr,
                      byte *output, int count,
                      const byte *range_limit,
                      const int *Crrtab, const int *Cbbtab,
                      const INT32 *Crgtab, const INT32 *Cbgtab)
{
	enum
	{
		ACCEL_BITS = 3,
		ACCEL_COUNT = 8,
	};

	static int y_buf[ACCEL_COUNT / 4], cb_buf[ACCEL_COUNT / 4], cr_buf[ACCEL_COUNT / 4];
	static char out_buf[ACCEL_COUNT * 3];
	static int __y;

	__asm mov       ebx, [range_limit]
	__asm sub       [count], ACCEL_COUNT
	__asm jb        __1
__4:
	__asm mov       esi, [y]
	__asm add       [y], ACCEL_COUNT
	__asm mov       eax, [esi + 0] ;; ACCEL_COUNT
	__asm mov       ecx, [esi + 4]
	__asm mov       [y_buf + 0], eax
	__asm mov       [y_buf + 4], ecx
	__asm mov       esi, [cr]
	__asm add       [cr], ACCEL_COUNT
	__asm mov       eax, [esi + 0] ;; ACCEL_COUNT
	__asm mov       ecx, [esi + 4]
	__asm mov       [cr_buf + 0], eax
	__asm mov       [cr_buf + 4], ecx
	__asm mov       esi, [cb]
	__asm add       [cb], ACCEL_COUNT
	__asm mov       eax, [esi + 0] ;; ACCEL_COUNT
	__asm mov       ecx, [esi + 4]
	__asm mov       [cb_buf + 0], eax
	__asm mov       [cb_buf + 4], ecx

#define REPEAT(macro) \
	macro(0) macro(1) macro(2) macro(3) \
	macro(4) macro(5) macro(6) macro(7)

	__asm mov       esi, [Crrtab]
	__asm mov       edi, [Cbbtab]
	__asm push      ebp

#define DO_RB(i) \
	__asm movzx     eax, byte ptr [y_buf + i] \
	__asm movzx     ecx, byte ptr [cr_buf + i] \
	__asm mov       ebp, eax \
	__asm movzx     edx, byte ptr [cb_buf + i] \
	__asm add       eax, [4 * ecx + esi] \
	__asm add       ebp, [4 * edx + edi] \
	__asm mov       al, [eax + ebx] \
	__asm mov       ah, [ebp + ebx] \
	__asm mov       [out_buf + 3 * i + 0], al \
	__asm mov       [out_buf + 3 * i + 2], ah

	REPEAT(DO_RB)
#undef DO_RB

	__asm pop       ebp
	__asm mov       esi, [Crgtab]
	__asm mov       edi, [Cbgtab]
	__asm push      ebp

#define DO_G(i) \
	__asm movzx     ecx, byte ptr [cr_buf + i] \
	__asm movzx     edx, byte ptr [cb_buf + i] \
	__asm mov       ebp, [4 * ecx + esi] \
	__asm movzx     eax, byte ptr [y_buf + i] \
	__asm add       ebp, [4 * edx + edi] \
	__asm sar       ebp, 16 \
	__asm add       eax, ebp \
	__asm mov       al, [eax + ebx] \
	__asm mov       [out_buf + 3 * i + 1], al

	REPEAT(DO_G)
#undef DO_G

	__asm pop       ebp
	__asm mov       edi, [output]
	__asm mov       eax, dword ptr [out_buf + 0] ;; ACCEL_COUNT
	__asm mov       ecx, dword ptr [out_buf + 4]
	__asm mov       edx, dword ptr [out_buf + 8]
	__asm mov       [edi + 0], eax
	__asm mov       [edi + 4], ecx
	__asm mov       [edi + 8], edx
	__asm mov       eax, dword ptr [out_buf + 12] ;; ACCEL_COUNT
	__asm mov       ecx, dword ptr [out_buf + 16]
	__asm mov       edx, dword ptr [out_buf + 20]
	__asm mov       [edi + 12], eax
	__asm mov       [edi + 16], ecx
	__asm mov       [edi + 20], edx
	__asm add       [output], ACCEL_COUNT * 3
	__asm sub       [count], ACCEL_COUNT
	__asm jnb       __4
__1:
	__asm and       [count], ACCEL_COUNT - 1
	__asm je        __2
	__asm mov       edi, [output]
__3:
	__asm mov       esi, [y]
	__asm movzx     eax, byte ptr [esi]
	__asm mov       [__y], eax
	__asm mov       esi, [cr]
	__asm movzx     edx, byte ptr [esi]
	__asm mov       ecx, [Crrtab]
	__asm mov       ecx, [4 * edx + ecx]
	__asm add       ecx, eax
	__asm mov       cl, [ecx + ebx]
	__asm mov       [edi], cl
	__asm mov       ecx, [Crgtab]
	__asm mov       esi, [cb]
	__asm mov       ecx, [4 * edx + ecx]
	__asm movzx     edx, byte ptr [esi]
	__asm mov       eax, [Cbgtab]
	__asm add       ecx, [4 * edx + eax]
	__asm mov       eax, [__y]
	__asm sar       ecx, 16
	__asm add       ecx, eax
	__asm mov       cl, [ecx + ebx]
	__asm mov       [edi + 1], cl
	__asm mov       ecx, [Cbbtab]
	__asm add       eax, [4 * edx + ecx]
	__asm mov       al, [eax + ebx]
	__asm mov       [edi + 2], al
	__asm add       edi, 3
	__asm inc       [y]
	__asm inc       [cb]
	__asm inc       [cr]
	__asm dec       [count]
	__asm jne       __3
__2:
	;
}

//////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif//COMPILER_MSC
