#ifndef _KHEAP
#define _KHEAP
#endif

#define NULL 0x0000000

// show kernel heap memory spec
void kheap_SPEC();

// points to start of kernel heap memory
void *KHEAP_START = (void *)0x0008000;

// points to end of total kernel heap memory
void *KHEAP_END = (void *)0x0080000;

// points to end of heap memory currently allocated
void *cur_heap = (void *)0x0008000;

// Each Memory block is represented with below struct
struct heap_block
{
        unsigned size;
        int free;
        struct heap_block *next;
        struct heap_block *prev;
        char data[1]; // points to actual data returned to kernel
};

// BLOCK SIZE contains malloc metadata
// char data[1] in struct heap_block, points to user requested allocation. 
// It will have a size of 4 because of padding. Hence we delete
// 4 from the sizeof struct
#define BLOCK_SIZE (sizeof(struct heap_block)-4)

// align memory requested to 4 bytes
// ((x-1)/4)*4 + 4
#define align(x)  ((((x-1)>>2)<<2)+4)

// extend heap
struct heap_block * extend_heap(struct heap_block *last, unsigned sz);

// allocate_block
struct heap_block * allocate_block(struct heap_block *last, unsigned sz);

// split_block
void split_block(struct heap_block *last, unsigned sz);

// malloc
void* kmalloc(unsigned sz);

// free
void kfree(void *fr);

// join adjacent free blocks
void fuse_block(struct heap_block *last);

