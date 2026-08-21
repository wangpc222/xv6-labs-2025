// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct {
  struct spinlock lock;
  struct run *freelist;
} spmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  initlock(&spmem.lock, "spmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  int supercount = 0;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE) {
    if(((uint64)p % SUPERPGSIZE) == 0 && p + SUPERPGSIZE <= (char*)pa_end) {
      supercount++;
      // First 4 superpage regions go to regular allocator (gives kmem ~8MB)
      if(supercount <= 4) {
        kfree(p);
      } else {
        superfree(p);
        p += SUPERPGSIZE - PGSIZE;
      }
    } else {
      kfree(p);
    }
  }
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}


// Free a 2MB superpage of physical memory.
void
superfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % SUPERPGSIZE) != 0 || (char*)pa < end || (uint64)pa + SUPERPGSIZE > PHYSTOP)
    panic("superfree");

  memset(pa, 1, SUPERPGSIZE);

  r = (struct run*)pa;

  acquire(&spmem.lock);
  r->next = spmem.freelist;
  spmem.freelist = r;
  release(&spmem.lock);
}

// Allocate one 2MB superpage of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
superalloc(void)
{
  struct run *r;

  acquire(&spmem.lock);
  r = spmem.freelist;
  if(r)
    spmem.freelist = r->next;
  release(&spmem.lock);

  if(r)
    memset((char*)r, 5, SUPERPGSIZE);
  return (void*)r;
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
