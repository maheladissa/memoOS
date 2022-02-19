// kheap.h -- Interface for kernel heap functions, also provides
//            a placement malloc() for use before the heap is 
//            initialised.
//            Written for JamesM's kernel development tutorials.

#ifndef KHEAP_H
#define KHEAP_H

#include "../common.h"

/**
   Allocate a chunk of memory, sz in size. If align == 1,
   the chunk must be page-aligned. If phys != 0, the physical
   location of the allocated chunk will be stored into phys.

   This is the internal version of kmalloc. More user-friendly
   parameter representations are available in kmalloc, kmalloc_a,
   kmalloc_ap, kmalloc_p.
**/
u32int kmalloc_int(u32int sz, int align, u32int *phys);

/**
   Allocate a chunk of memory, sz in size. The chunk must be
   page aligned.
**/
u32int kmalloc_a(u32int sz);

/**
   Allocate a chunk of memory, sz in size. The physical address
   is returned in phys. Phys MUST be a valid pointer to u32int!
**/
u32int kmalloc_p(u32int sz, u32int *phys);

/**
   Allocate a chunk of memory, sz in size. The physical address 
   is returned in phys. It must be page-aligned.
**/
u32int kmalloc_ap(u32int sz, u32int *phys);

/**
   General allocation function.
**/
u32int kmalloc(u32int sz);

// end is defined in the linker script.
extern u32int KERNEL_PHYSICAL_END;
u32int placement_address = (u32int)&KERNEL_PHYSICAL_END;

u32int kmalloc_int(u32int sz, int align, u32int *phys)
{
    // This will eventually call malloc() on the kernel heap.
    // For now, though, we just assign memory at placement_address
    // and increment it by sz. Even when we've coded our kernel
    // heap, this will be useful for use before the heap is initialised.
    if (align == 1 && (placement_address & 0xFFFFF000) )
    {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys)
    {
        *phys = placement_address;
    }
    u32int tmp = placement_address;
    placement_address += sz;
    return tmp;
}

u32int kmalloc_a(u32int sz)
{
    return kmalloc_int(sz, 1, 0);
}

u32int kmalloc_p(u32int sz, u32int *phys)
{
    return kmalloc_int(sz, 0, phys);
}

u32int kmalloc_ap(u32int sz, u32int *phys)
{
    return kmalloc_int(sz, 1, phys);
}

u32int kmalloc(u32int sz)
{
    return kmalloc_int(sz, 0, 0);
}

#endif // KHEAP_H
