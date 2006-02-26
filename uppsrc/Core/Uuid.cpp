#include "Core.h"

#pragma BLITZ_APPROVE

void Uuid::Serialize(Stream& s) {
	int version = 0;
	s / version % a % b %c % d;
}

#ifdef PLATFORM_WIN32

#define Ptr Ptr_
#include <objbase.h>
#undef Ptr

#pragma comment(lib, "ole32.lib")

Uuid Uuid::Create() {
	Uuid uuid;
	CoCreateGuid((GUID *)&uuid);
	return uuid;
}
#endif

#if defined(PLATFORM_POSIX)

static StaticCriticalSection sUuidLock;

Uuid Uuid::Create() {
	CriticalSection::Lock _(sUuidLock);
	static int fd;
	if(!fd)
		fd = open("/dev/urandom", O_RDONLY);
	ASSERT(fd > 0);
	static byte uuid[sizeof(Uuid)];
	if(uuid[0] == 0) {
		read(fd, uuid, sizeof(Uuid));
		uuid[0] = 255;
	}
	else
		uuid[0]--;
	return *(Uuid *)uuid;
}

#endif

String Format(const Uuid& id) {
	return Sprintf("%08X%08X%08X%08X", id.a, id.b, id.c, id.d);
}

String Dump(const Uuid& id) {
	return "UUID: " + Format(id);
}

struct UuidValueGenClass : ValueGen
{
	virtual Value Get() {
		return Format(Uuid::Create());
	}
};

ValueGen& UuidValueGen()
{
	return Single<UuidValueGenClass>();
}
