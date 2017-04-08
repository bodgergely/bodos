/*
 * rdtsc.h
 *
 *  Created on: Apr 8, 2017
 *      Author: geri
 */

#ifndef SRC_KERNEL_ARCH_I386_UTILS_RDTSC_H_
#define SRC_KERNEL_ARCH_I386_UTILS_RDTSC_H_


static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}


#endif /* SRC_KERNEL_ARCH_I386_UTILS_RDTSC_H_ */
