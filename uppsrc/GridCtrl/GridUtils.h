#ifndef _GridCtrl_GridUtils_h_
#define _GridCtrl_GridUtils_h_

#define BIT(x) (1 << x)

inline bool IsSet(int s, int b)
{
	return s & b;
}

inline void BitSet(dword &k, dword v, bool s)
{
	if(s) k |= v; else  k &= ~v;
}

inline bool BitInverse(dword &k, dword v)
{
	bool s = k & v;
	BitSet(k, v, !s);
	return s;
}

inline int Distance(const Point &p0, const Point &p1)
{
	return max(abs(p0.x - p1.x), abs(p0.y - p1.y));
}

inline int32 Round(double a)
{
	#ifdef flagGCC
	return (int32) a;
	#else
	int32 retval;

	__asm fld a
	__asm fistp retval
	return retval;

	#endif
}

#ifdef flagDEBUG
#define LG //Log
#else
#define LG
#endif
#define LGR Log

extern LineEdit *dlog;
extern DropList *dlev;

void Log(const char *fmt, ...);
void Log(int level, const char *fmt, ...);

#endif
