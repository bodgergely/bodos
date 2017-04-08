/*
 * randtest.cpp
 *
 *  Created on: Apr 8, 2017
 *      Author: geri
 */

#include <kernel/utils/random.h>
#include <kernel/klog.h>


void generateRandomNumbers(int count)
{
	for(int i=0;i<count;i++)
	{
		kprintf("%d\n", rand());
	}
}

