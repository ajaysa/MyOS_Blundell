#include <kheap.h>

// points to start of kernel heap
void *base = NULL;

void kheap_SPEC()
{
	print("Start of kernel heap memory :: ");
	print_hex(KHEAP_START);
	print("\nEnd of kernel heap memory   :: ");
	print_hex(KHEAP_END);
	print("\ncurrent pointer to kernel heap memory :: ");
	print_hex(cur_heap);
	print("\n");
}

struct heap_block * extend_heap(struct heap_block *last, unsigned sz)
{
	struct heap_block *temp;
	temp = (struct heap_block *)cur_heap;

	if (  ((char *)cur_heap + BLOCK_SIZE + sz) > (char *)KHEAP_END ) // all memory exhausted
		return NULL;

	temp->size = sz;
	temp->free = 0;
	temp->next = NULL;
	temp->prev = last;
	if (last)
		last->next = temp;

	// Update current memory pointer
	cur_heap = (void *)((char *)cur_heap + BLOCK_SIZE + sz);

	return temp;
}

void split_block(struct heap_block *last, unsigned sz)
{
	struct heap_block *new_block;

	if ( (last->size - sz) >= (BLOCK_SIZE + 4) ) // if left over memory big enough, then split
	{
		new_block = (struct heap_block *)(last->data + sz);
		new_block->size = last->size - sz - BLOCK_SIZE;
		new_block->free = 1;
		new_block->next = last->next;
		new_block->prev = last;

		last->next = new_block;
		last->size = sz;
	}
}

struct heap_block * allocate_block(struct heap_block *last, unsigned sz)
{
	struct heap_block *temp, *prev;
	temp = last;
	prev = NULL;
	while(temp)
	{
		if ( temp->free == 1 && temp->size >= sz )
		{
			split_block(temp, sz); // call split block, split in case large memory
			temp->free = 0;
			return temp;
		}

		prev = temp;
		temp = temp->next;
	}

	return extend_heap(prev, sz); // if no free block large enough found, call extend heap
}

void* kmalloc(unsigned sz)
{
	struct heap_block *temp;
	unsigned sz_align = align(sz);

	if ( base )
		temp = allocate_block(base, sz_align);
	else
	{
		temp = extend_heap(base, sz_align);
		base = temp; // initialize base pointer
	}

	if ( temp )
		return temp->data; // data points to user requested space
	else
		return NULL;
}

void fuse_block(struct heap_block *last)
{
	struct heap_block *temp;

	// fuse the next blocks is free
	temp = last->next;
	while(temp)
	{
		if ( temp->free == 1 )
		{
			last->size += BLOCK_SIZE + (last->next)->size;
			last->next = temp->next;

			temp = last->next;
		}
		else
			break;
	}

	// fuse the prev blocks is free
	temp = last->prev;
	while(temp)
	{
		if ( temp->free == 1 )
		{
			temp->size += BLOCK_SIZE + last->size;
			temp->next = last->next;
			last = temp;

			temp = last->prev;
		}
		else
			break;
	}
}

void kfree(void *fr)
{
	struct heap_block *temp;

	// point to start of meta info
	temp = (struct heap_block *)((char *)fr - BLOCK_SIZE);
	temp->free = 1;

	// fuse blocks if possible
	fuse_block(temp);
}
