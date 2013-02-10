#include "InVector.h"



CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	SeedRandom();
	SortedIndex<int> si;
	
	for(int i = 0; i < 100; i++)
		si.Add(Random(20));
	
	DDUMPC(si);
}
