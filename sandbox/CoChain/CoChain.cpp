#include "CoChain.h"

#define LLOG(x) LOG(x)

static thread__ int targeti__;

void CoChain::Schedule()
{
	for(int i = 0; i < node.GetCount(); i++) {
		LLOG(i << ": " << node[i].work.GetCount() << " " << node[i].processing);
		DDUMP((i + 1 >= node.GetCount() || node[i + 1].work.GetCount() < 8));
		if(!node[i].processing && node[i].work.GetCount() &&
		    (i + 1 >= node.GetCount() || node[i + 1].work.GetCount() < 8)) { // Schedule ready to go nodes for processing
			node[i].processing = true;
			mutex.Leave();
			LLOG("Scheduling " << i);
			work & THISBACK1(ProcessNode, i);
			mutex.Enter();
		}
	}
}

void CoChain::ProcessNode(int i)
{
	LLOG("ProcessNode " << i << " in thread " << Thread::GetCurrentId());
	mutex.Enter();
	Callback cb = node[i].work.Tail();
	node[i].work.DropTail();
	mutex.Leave();
	int h = targeti__;
	targeti__ = i + 1;
	cb();
	targeti__ = h;
//	LLOG("-ProcessNode " << i << " in thread " << Thread::GetCurrentId());
	mutex.Enter();
	node[i].processing = false;
	Schedule();
	todo--;
	if(blocked && i == 0)
		sem.Release();
	mutex.Leave();
}

void CoChain::Do(Callback cb)
{
	Mutex::Lock __(mutex);
	Node& n = node.At(targeti__);
	while(targeti__ == 0 && n.work.GetCount() >= 8) {
		blocked = true;
		mutex.Leave();
		sem.Wait();
		mutex.Enter();
		blocked = false;
	}
	node.At(targeti__).work.AddHead(cb);
	LLOG("Do " << targeti__ << " queue size " << node[targeti__].work.GetCount());
	todo++;
	Schedule();
}


void CoChain::Finish()
{
	while(todo) {
		work.Finish();
		Mutex::Lock __(mutex);
		Schedule();
	}
}

CoChain::CoChain() {
	targeti__ = 0; // Resolve this!
    todo = 0;
    blocked = false;
}

CoChain::~CoChain()
{
	Finish();
}
