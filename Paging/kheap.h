#ifndef KHEAP_H
#define KHEAP_H

// from http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html

#include "../common.h"

extern u32int kernel_end;
u32int placement_address = (u32int)&kernel_end;

u32int kmalloc(u32int size) {
	u32int mem =  placement_address;
	placement_address += size;
	return mem;
}

u32int kmalloc_page() {
	if (placement_address & 0xFFFFF000) { // not alligned 
        	// Align the placement address;
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
    	}
	return kmalloc(0x1000);
}
#endif
