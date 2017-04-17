#pragma once

#include <kernel/klog.h>

extern "C"
{

void ctxswitch(void* oldSP, void* newSP);


}
