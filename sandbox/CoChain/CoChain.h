#include <Core/Core.h>

using namespace Upp;

struct CoChain {
	CoWork    work;
	Mutex     mutex;
	int       todo;
	Semaphore sem;
	bool      blocked;

	struct Node {
		bool               processing;
		BiVector<Callback> work;
		
		Node() { processing = false; }
	};

	Array<Node> node;
	
	typedef CoChain CLASSNAME;
	
	void Schedule();
	void ProcessNode(int i);
	void Do(Callback cb);
	
	void operator()(Callback cb) { Do(cb); }
	
	void Finish();
	
	CoChain();
	~CoChain();
};

