#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "nu_malloc.h"

void* base = NULL;

void* nu_malloc(size_t size) {
	t_block b, last;
	size_t s;
	
	s = align4(size);
	
	if(base) {
		last = base;
		b = find_block(&last, s);
		
		if(b) {
			if((b->size-s) >= (BLOCK_SIZE+4))
				split_block(b, s);
			b->free = 0;
		}
	else {
		b = extend_heap(last, s);
		
		if(!b)
			return NULL;
		base = b;
	}
	return(b->data);
}

void* nu_calloc(size_t count, size_t size) {
	size_t *new;
	size_t s4, i;
	new = nu_malloc(count * size);
	
	if(new) {
		s4 = align4(count * size) << 2;
		for(i = 0; i < s4; i++)
		new[i] = 0;
	}
	return (new);  
}

void* nu_realloc(void* ptr, size_t size) {
	
  return NULL; 
}

// Finds a block of free space in memory. Returns the block
// or NULL if there is not a large enough block.
t_block find_block(t_block *last, size_t size) {
	t_block b  = base;
	
	while(b && !(b->free && b->size >= size)) {
		*last = b;
		b = b->next;
	}
	return (b);
}

// Extends the heap.  Finds the current break point location
// and moves it to the next place depending on the size.
// Returns NULL upon failure.  Returns the block upon success.
t_block extend_heap(t_block last, size_t s) {
	t_block b;
	b = sbrk(0);
	
	if(sbrk(BLOCK_SIZE + s) == (void*)-1)
		return NULL;
	b->size = s;
	b->next = NULL;
	
	if(last)
		last->next = b;
	b->free = 0;
	
	return (b);
}

void split_block(t_block b, size_t s) {
	t_block new;
	new = b->data + s;
	new->size = b->size - s - BLOCK_SIZE;
	new->next = b->next;
	new->free = 1;
	b->size = s;
	b->next = new;
}

void nu_free(void* ptr){
  
}
