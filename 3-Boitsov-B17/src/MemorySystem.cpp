#include<stdio.h>
#include"memallocator.h"

#define d_size (2 * sizeof(int) + 2 * sizeof(void*))


void* memory;
int initialsize;
void* head;
void* tail;
char direction_switch = 1;

int myabs(int a)
{
	if (a >= 0)
		return a;
	else
		return a * (-1);
}

int memgetminimumsize()
{
	return 2 * sizeof(int) + 2 * sizeof(void*);
}

int memgetblocksize()
{
	return 2 * sizeof(int) + 2 * sizeof(void*);
}

int* getsize(void* block) {
	return (int*)(block);
}

int* isUsed(void* block) {
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
	*isUsed(head) = -initialsize;
	return 1;

}

void* headsearch(void* block, int size) {
	void* currentblock = block;
	while (currentblock)
	{
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
	*isUsed(newblock) = -*getsize(newblock);
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
	*isUsed(block) = *getsize(block);
}

void* memalloc(int size) {
	if ((size + d_size > initialsize) || (head == NULL) || (size < 0))
		return NULL;
	void* currentblock;
	if (direction_switch) {
		currentblock = head;
		currentblock = headsearch(currentblock, size);
		direction_switch = 0;
		if (currentblock == NULL)
			return NULL;
	}
	else {
		currentblock = tail;
		currentblock = tailsearch(currentblock, size);
		direction_switch = 1;
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
		*isUsed(currentblock) = *getsize(currentblock);
	}
	return (void*)((char*)currentblock + d_size - sizeof(int));
}

void  memfree(void* p) {
	if (p == NULL)
		return;
	void* pn = (void*)((char*)p - d_size + sizeof(int));
	if (*isUsed(pn) < 0)
		return;
	void* left = NULL;
	void* right = NULL;
	if ((int*)((char*)pn - 1) >= (int*)memory)
		left = (void*)((char*)pn - myabs(*(int*)((char*)pn - sizeof(int))));
	if ((char*)pn + *getsize(pn) < (char*)memory + initialsize)
		right = (void*)((char*)pn + *getsize(pn));
	*isUsed(pn) = -*getsize(pn);
	if ((left != NULL) && (*isUsed(left) < 0)) {
		if ((right != NULL) && (*isUsed(right) < 0)) {
			if (*getnext(right) == NULL) {
				tail = *getprev(tail);
				*getnext(tail) = NULL;
			}
			else {
				if (*getprev(right) == NULL) {
					head = *getnext(head);
					*getprev(head) = NULL;
				}
				else {
					*getnext(*getprev(right)) = *getnext(right);
					*getprev(*getnext(right)) = *getprev(right);
				}
			}
			*getnext(right) = NULL;
			*getprev(right) = NULL;
			*getsize(left) = *getsize(left) + *getsize(right) + *getsize(pn);
			*isUsed(left) = -*getsize(left);
		}
		else {
			*getsize(left) = *getsize(left) + *getsize(pn);
			*isUsed(left) = -*getsize(left);
		}
	}
	else {
		if ((right != NULL) && (*isUsed(right) < 0)) {
			*getprev(pn) = *getprev(right);
			*getnext(pn) = *getnext(right);
			if (*getnext(right) == NULL) {
				if (*getprev(right) == NULL) {
					head = pn;
					tail = pn;
				}
				else {
					tail = pn;
					*getnext(*getprev(right)) = pn;
				}
			}
			else {
				if (*getprev(right) == NULL) {
					head = pn;
					*getprev(*getnext(right)) = pn;
				}
				else {
					*getnext(*getprev(right)) = pn;
					*getprev(*getnext(right)) = pn;
				}
			}
			*getnext(right) = NULL;
			*getprev(right) = NULL;
			*getsize(pn) = *getsize(pn) + *getsize(right);
			*isUsed(pn) = -*getsize(pn);
		}
		else {
			if (head == NULL) {
				head = pn;
				tail = pn;
				return;
			}
			*getnext(tail) = pn;
			*getprev(pn) = tail;
			tail = pn;
		}
	}
}

void memdone() {
	void* currentblock = memory;
	while ((char*)currentblock < (char*)memory + initialsize)
	{
		if (*getsize(currentblock) > 0)
			fprintf(stderr, "MEMORY LEAK:\tBlock 0x%p; size: %i bytes.\n", currentblock, *getsize(currentblock));
		currentblock = (void*)((char*)currentblock + *getsize(currentblock));
	}
}