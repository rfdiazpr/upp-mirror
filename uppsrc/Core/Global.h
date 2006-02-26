#define GLOBAL_V(type, name) \
name() \
{ \
	static type *q; \
	if(!q) { \
		static StaticCriticalSection cs; \
		CriticalSection::Lock __(cs); \
		if(!q) { \
			static type x; \
			q = &x; \
		} \
	} \
	return *q; \
}

#define GLOBAL_VAR(type, name) \
type& GLOBAL_V(type, name)

#define GLOBAL_VP(type, name, param) \
name() { \
	static type *q; \
	if(!q) { \
		static StaticCriticalSection cs; \
		CriticalSection::Lock __(cs); \
		if(!q) { \
			static type x param; \
			q = &x; \
		} \
	} \
	return *q; \
}

#define GLOBAL_VARP(type, name, param) \
type& GLOBAL_VP(type, name, param)


// DEPRECATED! (USE ONCELOCK_)
#define INIT_LOCKV(init, code) \
if(!init) { \
	static StaticCriticalSection cs; \
	CriticalSection::Lock __(cs); \
	if(!init) { \
		init = true; \
		code \
	} \
}

// DEPRECATED! (USE ONCELOCK)
#define INIT_LOCK(code) \
static bool init; \
INIT_LOCKV(init, code)


void Set__(volatile bool& b);

#define ONCELOCK \
for(static volatile bool o_b_; !o_b_;) \
	for(static StaticCriticalSection o_ss_; !o_b_;) \
		for(CriticalSection::Lock o_ss_lock__(o_ss_); !o_b_; Set__(o_b_))

#define ONCELOCK_(o_b_) \
for(static StaticCriticalSection o_ss_; !o_b_;) \
	for(CriticalSection::Lock o_ss_lock__(o_ss_); !o_b_; Set__(o_b_))
