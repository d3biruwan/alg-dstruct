#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

	typedef struct {
		int* neighboursarray;
		int size;
	} neighbours_t;

	typedef struct {
		neighbours_t* adjacencyarray;
		int size;
	}   graph_t;

	typedef struct node_t {
		int data;
		struct node_t* next;
	}node_t;

	typedef struct {
		node_t* top;
	}stack_t;

	stack_t* stackinit();
	int stackisempty(stack_t* stack);
	int stackpush(stack_t* stack, int vertex);
	int stackpop(stack_t* stack);
	void stackdestroy(stack_t* stack);
	graph_t* createbasegraph(int size);
	void graphdestroy(graph_t* graph);
	graph_t* readgraph(FILE* file);
	int depthfirstsearch(FILE* file, graph_t* graph);
	int lab(void);

#ifdef __cplusplus
}
#endif