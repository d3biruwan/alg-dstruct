#include<stdio.h>
#include"memallocator.h"

#define d_size (2 * sizeof(int) + 2 * sizeof(void*))


void* memory;
int initialsize;
void* head;
void* tail;
char directionswitch = 1;

int myabs(int a) {
	if (a >= 0)
		return a;
	else
		return a * (-1);
}

int memgetminimumsize() {
	return 2 * sizeof(int) + 2 * sizeof(void*);
}

int memgetblocksize() {
	return 2 * sizeof(int) + 2 * sizeof(void*);
}

int* getsize(void* block) {
	return (int*)(block);
}

int* getusedflag(void* block) {
	return (int*)((char*)block + *getsize(block) - sizeof(int));
}

void** getnext(void* block) {
	return (void**)((char*)block + sizeof(int));
}

void** getprev(void* block) {
	return (void**)((char*)block + sizeof(int) + sizeof(void*));
}

int meminit(void* pMemory, int size) {
	if ((pMemory == NULL) || (size <= d_size))
		return 0;
	memory = pMemory;
	initialsize = size;
	head = pMemory;
	tail = pMemory;
	*getsize(head) = initialsize;
	*getnext(head) = NULL;
	*getprev(head) = NULL;
	*getusedflag(head) = -initialsize;
	return 1;

}

void* headsearch(void* block, int size) {
	void* currentblock = block;
	while (currentblock) {
		if (*getsize(currentblock) >= size + d_size)
			return (void*)currentblock;
		currentblock = *getnext(currentblock);
	}
	return NULL;
}

void* tailsearch(void* block, int size) {
	void* currentblock = block;
	while (currentblock) {
		if (*getsize(currentblock) >= size + d_size)
			return (void*)currentblock;
		currentblock = *getprev(currentblock);
	}
	return NULL;
}

void makenewblock(void* block, int size) {
	void* newblock = (void*)((char*)block + size + d_size);
	*getprev(newblock) = *getprev(block);
	*getnext(newblock) = *getnext(block);
	*getsize(newblock) = *getsize(block) - size - d_size;
	*getusedflag(newblock) = -*getsize(newblock);
	if (*(getprev(block)) == NULL) {
		if (*(getnext(block)) == NULL) {

			head = newblock;
			tail = newblock;
		}
		else {
			*getprev(*getnext(block)) = newblock;
			head = newblock;
		}
	}
	else {
		if (*(getnext(block)) == NULL) {
			*getnext(*getprev(block)) = newblock;
			tail = newblock;
		}
		else {
			*getnext(*getprev(block)) = newblock;
			*getprev(*getnext(block)) = newblock;
		}
	}
	*getsize(block) = size + d_size;
	*getnext(block) = NULL;
	*getprev(block) = NULL;
	*getusedflag(block) = *getsize(block);
}

void* memalloc(int size) {
	if ((size + d_size > initialsize) || (head == NULL) || (size < 0))
		return NULL;
	void* currentblock;
	if (directionswitch) {
		currentblock = head;
		currentblock = headsearch(currentblock, size);
		directionswitch = 0;
		if (currentblock == NULL)
			return NULL;
	}
	else {
		currentblock = tail;
		currentblock = tailsearch(currentblock, size);
		directionswitch = 1;
		if (currentblock == NULL)
			return NULL;
	}
	if (*getsize(currentblock) > size + 2 * d_size)
		makenewblock(currentblock, size);
	else {
		if (*getprev(currentblock) == NULL) {
			if (*getnext(currentblock) == NULL) {
				tail = NULL;
				head = NULL;
			}
			else {
				head = *getnext(head);
				*getprev(head) = NULL;
			}
		}
		else {
			if (*getnext(currentblock) == NULL) {
				tail = *getprev(tail);
				*getnext(tail) = NULL;
			}
			else {
				*getnext(*getprev(currentblock)) = *getnext(currentblock);
				*getprev(*getnext(currentblock)) = *getprev(currentblock);
			}
		}
		*getnext(currentblock) = NULL;
		*getprev(currentblock) = NULL;
		*getusedflag(currentblock) = *getsize(currentblock);
	}
	return (void*)((char*)currentblock + d_size - sizeof(int));
}

void memfree(void* p) {
	if (p == NULL)
		return;
	void* block = (void*)((char*)p - d_size + sizeof(int));
	if (*getusedflag(block) < 0)
		return;
	void* leftblock = NULL;
	void* rightblock = NULL;
	char left_is_free, right_is_free;
	if ((int*)((char*)block - 1) >= (int*)memory)
		leftblock = (void*)((char*)block - myabs(*(int*)((char*)block - sizeof(int))));
	if ((char*)block + *getsize(block) < (char*)memory + initialsize)
		rightblock = (void*)((char*)block + *getsize(block));
	*getusedflag(block) = -*getsize(block);
	if (leftblock == NULL)
		left_is_free = 0;
	else
		if (*getusedflag(leftblock) < 0)
			left_is_free = 1;
		else
			left_is_free = 0;
	if (rightblock == NULL)
		right_is_free = 0;
	else
		if (*getusedflag(rightblock) < 0)
			right_is_free = 1;
		else
			right_is_free = 0;
	if (left_is_free) {
		if (right_is_free) {
			if (*getnext(rightblock) == NULL) {
				tail = *getprev(tail);
				*getnext(tail) = NULL;
			}
			else {
				if (*getprev(rightblock) == NULL) {
					head = *getnext(head);
					*getprev(head) = NULL;
				}
				else {
					*getnext(*getprev(rightblock)) = *getnext(rightblock);
					*getprev(*getnext(rightblock)) = *getprev(rightblock);
				}
			}
			*getnext(rightblock) = NULL;
			*getprev(rightblock) = NULL;
			*getsize(leftblock) = *getsize(leftblock) + *getsize(rightblock) + *getsize(block);
			*getusedflag(leftblock) = -*getsize(leftblock);
		}
		else {
			*getsize(leftblock) = *getsize(leftblock) + *getsize(block);
			*getusedflag(leftblock) = -*getsize(leftblock);
		}
	}
	else {
		if (right_is_free) {
			*getprev(block) = *getprev(rightblock);
			*getnext(block) = *getnext(rightblock);
			if (*getnext(rightblock) == NULL) {
				if (*getprev(rightblock) == NULL) {
					head = block;
					tail = block;
				}
				else {
					tail = block;
					*getnext(*getprev(rightblock)) = block;
				}
			}
			else {
				if (*getprev(rightblock) == NULL) {
					head = block;
					*getprev(*getnext(rightblock)) = block;
				}
				else {
					*getnext(*getprev(rightblock)) = block;
					*getprev(*getnext(rightblock)) = block;
				}
			}
			*getnext(rightblock) = NULL;
			*getprev(rightblock) = NULL;
			*getsize(block) = *getsize(block) + *getsize(rightblock);
			*getusedflag(block) = -*getsize(block);
		}
		else {
			if (head == NULL) {
				head = block;
				tail = block;
				return;
			}
			*getnext(tail) = block;
			*getprev(block) = tail;
			tail = block;
		}
	}
}

void memdone() {
	void* currentblock = memory;
	while ((char*)currentblock < (char*)memory + initialsize) {
		if (*getusedflag(currentblock) > 0)
			fprintf(stderr, "MEMORY LEAK:\tBlock 0x%p; size: %i bytes.\n", currentblock, *getsize(currentblock));
		currentblock = (void*)((char*)currentblock + *getsize(currentblock));
	}
}