#include"Header.h" 

stack* stackinit() {
    stack* Q = (stack*)malloc(sizeof(stack));
    if (Q == NULL)
        return NULL;
    Q->top = NULL;
    return Q;
}

int stackisempty(stack* Q) {
    if (Q->top == NULL)
        return 1;
    else
        return 0;
}

int stackpush(stack* Q, int vertex) {
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if (node == NULL)
        return 0;
    node->data = vertex;
    node->next = Q->top;
    Q->top = node;
    return 1;
}

int stackpop(stack* Q) {
    if (!stackisempty(Q)) {
        node_t* node = Q->top;
        Q->top = Q->top->next;
        free(node);
        return 1;
    }
    return 0;
}

void stackdestroy(stack* Q) {
    node_t* currentnode = Q->top;
    while (currentnode != NULL) {
        node_t* freenode = currentnode;
        currentnode = currentnode->next;
        free(freenode);
    }
    free(Q);
}

graph* createbasegraph(int size) {

    graph* g = (graph*)malloc(sizeof(graph));
    if (g == NULL)
        return NULL;
    g->size = size;
    neighbours* adjarr = (neighbours*)malloc(sizeof(neighbours) * size);
    if (adjarr == NULL)
        return NULL;
    for (int i = 0; i < size; i++) {
        adjarr[i].size = 0;
        adjarr[i].neighboursarray = NULL;
    }
    g->adjacencyarray = adjarr;
    return g;
}

void graphdestroy(graph* g) {
    if (g == NULL)
        return;
    for (int i = 0; i < g->size; i++)
        if (g->adjacencyarray[i].neighboursarray != NULL)
            free(g->adjacencyarray[i].neighboursarray);
    free(g->adjacencyarray);
    free(g);
}

graph* readgraph(FILE* f) {
    int size, vertex;
    fscanf(f, "%d", &size);
    graph* g = createbasegraph(size);
    if (g == NULL)
        return NULL;
    for (int i = 0; i < size; i++) {
        fscanf(f, "%d", &vertex);
        int currentsize = g->adjacencyarray[i].size;
        while ((!feof(f)) && (fgetc(f) == ' ')) {
            g->adjacencyarray[i].size += 1;
            currentsize++;
            int* temp = g->adjacencyarray[i].neighboursarray;
            temp = (int*)realloc(temp, sizeof(int) * g->adjacencyarray[i].size);
            if (temp == NULL) {
                graphdestroy(g);
                return NULL;
            }
            g->adjacencyarray[i].neighboursarray = temp;
            fscanf(f, "%d", &g->adjacencyarray[i].neighboursarray[currentsize - 1]);
            g->adjacencyarray[g->adjacencyarray[i].neighboursarray[currentsize - 1]].size += 1;
            int* adjvertexneighbours = g->adjacencyarray[g->adjacencyarray[i].neighboursarray[currentsize - 1]].neighboursarray;
            int adjvertexsize = g->adjacencyarray[g->adjacencyarray[i].neighboursarray[currentsize - 1]].size;
            temp = (int*)realloc(adjvertexneighbours, sizeof(int) * adjvertexsize);
            if (temp == NULL) {
                graphdestroy(g);
                return NULL;
            }
            g->adjacencyarray[g->adjacencyarray[i].neighboursarray[currentsize - 1]].neighboursarray = temp;
            g->adjacencyarray[g->adjacencyarray[i].neighboursarray[currentsize - 1]].neighboursarray[adjvertexsize - 1] = vertex;
        }
    }
    return g;
}

int DFS(FILE* f, graph* g) {
    if (g == NULL)
        return 0;
    stack* Q = stackinit();
    if (Q == NULL) {
        stackdestroy(Q);
        return 0;
    }
    int currentvertex = 0;
    int size = g->size;
    if (size == 0) {
        stackdestroy(Q);
        return 0;
    }
    char* visitflag = (char*)malloc(size);
    if (visitflag == NULL) {
        stackdestroy(Q);
        return 0;
    }
    visitflag[0] = 1;
    fprintf(f, "%d ", 0);
    for (int i = 1; i < size; i++)
        visitflag[i] = 0;
    for (int i = g->adjacencyarray[currentvertex].size - 1; i >= 0; i--)
        if (!stackpush(Q, g->adjacencyarray[currentvertex].neighboursarray[i])) {
            free(visitflag);
            stackdestroy(Q);
            return 0;
        }
    while (!stackisempty(Q)) {
        currentvertex = Q->top->data;
        stackpop(Q);
        if (!visitflag[currentvertex]) {
            fprintf(f, "%d ", currentvertex);
            visitflag[currentvertex] = 1;
            for (int i = g->adjacencyarray[currentvertex].size - 1; i >= 0; i--)
                if (!stackpush(Q, g->adjacencyarray[currentvertex].neighboursarray[i])) {
                    free(visitflag);
                    stackdestroy(Q);
                    return 0;
                }
        }
    }
    free(visitflag);
    stackdestroy(Q);
    return 1;
}

int lab(void) {
    graph* g = readgraph(stdin);
    if (g == NULL)
        return 1;
    if (!DFS(stdout, g)) {
        graphdestroy(g);
        return 1;
    }
    graphdestroy(g);
    return 0;
}

