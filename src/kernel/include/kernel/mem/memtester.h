/*
 * memtester.h
 *
 *  Created on: Apr 6, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_
#define SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_

#include <kernel/klog.h>
#include <kernel/mem/kmalloc.h>
#include <string.h>

namespace memory
{

// tests

class MemTester
{
public:
	bool run()
	{
		performAllocs(1000);
	}
private:
	void performAllocs(int count)
	{
		for(int i=0;i<count;i++)
		{
			void* mem = kmalloc(10);
			strcpy((char*)mem, "Vivek");
			kprintf((char*)mem);
		}
	}
};


}



#endif /* SRC_KERNEL_INCLUDE_KERNEL_MEM_MEMTESTER_H_ */
