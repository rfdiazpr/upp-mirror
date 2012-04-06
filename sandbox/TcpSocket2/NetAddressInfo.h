#ifndef _TcpSocket2_NetAddressInfo_h_
#define _TcpSocket2_NetAddressInfo_h_

class IpAddressInfo {
	enum { COUNT = 32 };
	struct Entry {
		const char *url;
		int         port;
		int         status;
		addrinfo   *addr;
	};
	static Entry     entry[COUNT];
	
	enum {
		EMPTY, WORKING, CANCELED, FOUND, ERROR
	};

	String url;
	int    pi;

	static void EnterPool();
	static void LeavePool();
	static void Thread(int pi);

	void Start();
	
	const addrinfo *REQUEST = reinterpret_cast<addrinfo *>(1);

public:
	void      Start(const String& url);
	bool      InProgress();
	addrinfo *GetResult();

	IpAddressInfo()            { pi = -1; }
	~IpAddressInfo();
};

#endif
