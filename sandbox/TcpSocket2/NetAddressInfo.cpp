#include "HttpRequest2.h"

addrinfo *IpAddressInfo::pool[32];

StaticMutex IpAddressInfoPoolMutex;

void IpAddressInfo::Thread(int pi)
{
	char h[1024];
	int  port;
	
	EnterPool();
	Entry& e = 
	strcpy(url, h.url
	

	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo *result;
	if(getaddrinfo(host, ~AsString(port), &hints, &result) == 0 && result) {
		EnterPool();
		if(e.status == WORKING) {
			e.addr = result;
			e.status = FINISHED;
		}
		else
			freeaddrinfo(e.addr);
	}
	else {
		EnterPool();
		e.status = ERROR;
	}
	LeavePool();
}

void IpAddressInfo::EnterPool()
{
	IpAddressInfoPoolMutex.Enter();
}

void IpAddressInfo::LeavePool()
{
	IpAddressInfoPoolMutex.Leave();
}

void IpAddressInfo::Start()
{
	if(pi >= 0)
		return;
	EnterPool();
	for(int i = 0; i < COUNT; i++)
		if(entry[i] == EMPTY) {
			status[i] = WORKING;
			StartThread(Thread, i);
			break;
		}
	LeavePool();
}

bool IpAddressInfo::InProgress()
{
	if(pi < 0) {
		Start();
		return true;
	}
	EnterPool();
	int s = status[pi];
	LeavePool();
	return s = WORKING;
}

addrinfo * IpAddressInfo::GetResult()
{
	addrinfo *f;
	if(pi >= 0) {
		EnterPool();
		if(entry[pi].status == FOUND) {
			f = entry[i].`
		}
		LeavePool();
	}
	return pi >= 0 && status[pi] == FOUND
}

IpAddressInfo::~IpAddressInfo()
{
	if(pi >= 0) {
		EnterPool();
		Entry& e = entry[pi];
		if(e.status == FOUND && e.addr)
			freeaddrinfo(e.addr);
		if(e.status == WORKING)
			e.status = CANCELED;
		e.addr = NULL;
		e.url = NULL;
		e.port = 0;
		LeavePool();
	}
}
