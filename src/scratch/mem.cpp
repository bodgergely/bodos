#include "alloc.h"

int main()
{
	init();
	MemoryTester memtester;
	memtester.run();
	return 0;
}
