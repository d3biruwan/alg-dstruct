#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define _CRT_SECURE_NO_WARNINGS

    #include<stdio.h>
    #include<stdlib.h>

    typedef struct {
        int* neighboursarray;
        int size;
    } neighbours;

    typedef struct {
        neighbours* adjacencyarray;
        int size;
    }   graph;

    typedef struct node_t {
        int data;
        struct node_t* next;
    }node_t;

    typedef struct {
        node_t* top;
    }stack;

    stack* stackinit();
    int stackisempty(stack* Q);
    int stackpush(stack* Q, int vertex);
    int stackpop(stack* Q);
    void stackdestroy(stack* stack);
    graph* createbasegraph(int size);
    void graphdestroy(graph* g);
    graph* readgraph(FILE* f);
    int DFS(FILE* f, graph* g);
    int lab(void);

#ifdef __cplusplus
}
#endif