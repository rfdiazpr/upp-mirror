enum LookOp {
	LOOK_PAINT,
	LOOK_PAINTEDGE,
	LOOK_MARGINS,
	LOOK_ISOPAQUE,
};

void  ChLookFn(Value (*fn)(Draw& w, const Rect& r, const Value& look, int lookop));

void  ChSet(const char *id, int i, const Value& v);
void  ChSet(const char *id, const Value& v);
void  ChSetf(const char *name, Value (*fn)(int));
Value ChGet(const char *name, int i);
Value ChGet(const char *name);

Image AdjustColors(const Image& img);

void   Override(Iml& target, Iml& source, bool colored = false);
void   ColoredOverride(Iml& target, Iml& source);

void   ChReset();

void   ChPaint(Draw& w, const Rect& r, const Value& look);
void   ChPaint(Draw& w, int x, int y, int cx, int cy, const Value& look);
void   ChPaintEdge(Draw& w, const Rect& r, const Value& look);
void   ChPaintEdge(Draw& w, int x, int y, int cx, int cy, const Value& look);
Rect   ChMargins(const Value& look);
bool   ChIsOpaque(const Value& look);

//private:
void ChRegisterVar__(const char *name, int n,
                     byte *state, Value (*get)(int i), void (*set)(int i, const Value& v));
void  ChSolve__(bool *state, void (*init)());

template <class T>
struct ChIniter__ {
	T          *var;
	int         ii;
	int         nn;

	ChIniter__& operator<<(const T& val) {
		ASSERT(ii >= 0 && ii < nn);
		for(int i = ii; i < nn; i++)
			var[i] = val;
		ii++;
		return *this;
	}
	ChIniter__& operator<<(T (*fn)(int)) {
		for(int i = 0; i < nn; i++)
			var[i] = (*fn)(i);
		ii = nn;
		return *this;
	}
	ChIniter__& operator<<(const Vector<T>& vals) {
		for(int i = 0; i < nn; i++)
			var[i] = vals[i];
		ii = nn;
		return *this;
	}
};

#define CH__(type, v, n, init)\
static byte  ch_##v##__state; \
static type *ch##v() {\
	static type x[n];\
	return x;\
}\
\
static void ch_sync__##v() {\
	if(ch_##v##__state)\
		return;\
	ch_##v##__state = 1;\
	ChIniter__<type> ww;\
	ww.var = ch##v();\
	ww.ii = 0;\
	ww.nn = n;\
	ww << init;\
}\
\
static Value ch_get__##v(int i)\
{\
	ASSERT(i >= 0 && i < n);\
	ch_sync__##v();\
	return ch##v()[i];\
}\
\
static void ch_set__##v(int i, const Value& v)\
{\
	ASSERT(i >= 0 && i < n);\
	ch##v()[i] = v;\
}\
\
INITBLOCK {\
	ChRegisterVar__(#v, n, &ch_##v##__state, ch_get__##v, ch_set__##v);\
}\

#define CH_S(type, v, n, init) CH__(type, v, n, init) \
type v(int ii) { ASSERT(ii >= 0 && ii <= n); ch_sync__##v(); return ch##v()[ii]; }

#define CH_(type, v, init) CH__(type, v, 1, init) \
type v() { ch_sync__##v(); return ch##v()[0]; }

#define CH_INT(v, init)         CH_(int, v, init)
#define CH_INTS(v, n, init)     CH_S(int, v, n, init)

#define CH_STRING(v, init)      CH_(String, v, init)
#define CH_STRINGS(v, n, init)  CH_S(String, v, n, init)

#define CH_COLOR(v, init)       CH_(Color, v, init)
#define CH_COLORS(v, n, init)   CH_S(Color, v, n, init)

#define CH_FONT(v, init)        CH_(Font, v, init)
#define CH_FONTS(v, n, init)    CH_S(Font, v, n, init)

#define CH_IMAGE(v, init)       CH_(Image, v, init)
#define CH_IMAGES(v, n, init)   CH_S(Image, v, n, init)

#define CH_LOOK(v, init)        CH_(Value, v, init)
#define CH_LOOKS(v, n, init)    CH_S(Value, v, n, init)


void ChPaint(Draw& w, const Rect& r, const Value& element);
void ChPaint(Draw& w, int x, int y, int cx, int cy, const Value& element);

Value ChLookWith(const Value& look, const Image& img);
Value ChLookWith(const Value& look, const Image& img, Color color);
Value ChLookWith(const Value& look, const Image& img, Color (*color)(int i), int i);
