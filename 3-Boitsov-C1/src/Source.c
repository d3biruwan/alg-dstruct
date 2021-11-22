#include"Header.h" 

stack_t* stackinit() {
    stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
    if (stack == NULL)
        return NULL;
    stack->top = NULL;
    return stack;
}

int stackisempty(stack_t* stack) {
    if (stack->top == NULL)
        return 1;
    else
        return 0;
}

int stackpush(stack_t* stack , int vertex) {
    node_t* newnode = (node_t*)malloc(sizeof(node_t));
    if (newnode == NULL)
        return 0;
    newnode->data = vertex;
    newnode->next = stack->top;
    stack->top = newnode;
    return 1;
}

int stackpop(stack_t* stack) {
    if (!stackisempty(stack)) {
        node_t* node = stack->top;
        stack->top = stack->top->next;
        free(node);
        return 1;
    }
    return 0;
}

void stackdestroy(stack_t* stack) {
    node_t* currentnode = stack->top;
    while (currentnode != NULL) {
        node_t* freenode = currentnode;
        currentnode = currentnode->next;
        free(freenode);
    }
    free(stack);
}

graph_t* createbasegraph(int size) {
    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    if (graph == NULL)
        return NULL;
    graph->size = size;
    neighbours_t* adjacencyarray = (neighbours_t*)malloc(sizeof(neighbours_t) * size);
    if (adjacencyarray == NULL) {
        free(graph);
        return NULL;   
    }
    for (int i = 0; i < size; i++) {
        adjacencyarray[i].size = 0;
        adjacencyarray[i].neighboursarray = NULL;
    }
    graph->adjacencyarray = adjacencyarray;
    return graph;
}

void graphdestroy(graph_t* graph) {
    if (graph == NULL)
        return;
    for (int i = 0; i < graph->size; i++)
        if (graph->adjacencyarray[i].neighboursarray != NULL)
            free(graph->adjacencyarray[i].neighboursarray);
    free(graph->adjacencyarray);
    free(graph);
}

graph_t* readgraph(FILE* file) {
    int size,vertex;
    fscanf(file, "%d", &size);
    graph_t* graph = createbasegraph(size);
    if (graph == NULL)
        return NULL;
    for (int i = 0; i < size; i++) {
        fscanf(file, "%d", &vertex);
        int currentsize = graph->adjacencyarray[i].size;
        while ((!feof(file)) && (fgetc(file) == ' ')) {
            graph->adjacencyarray[i].size += 1;
            currentsize++;
            int* temp = graph->adjacencyarray[i].neighboursarray;
            temp = (int*)realloc(temp, sizeof(int) * graph->adjacencyarray[i].size);
            if (temp == NULL) {
                graphdestroy(graph);
                return NULL;
            }
            graph->adjacencyarray[i].neighboursarray = temp;
            fscanf(file, "%d", &graph->adjacencyarray[i].neighboursarray[currentsize - 1]);
            graph->adjacencyarray[graph->adjacencyarray[i].neighboursarray[currentsize - 1]].size += 1;
            int* adjvertexneighbours = graph->adjacencyarray[graph->adjacencyarray[i].neighboursarray[currentsize - 1]].neighboursarray;
            int adjvertexsize = graph->adjacencyarray[graph->adjacencyarray[i].neighboursarray[currentsize - 1]].size;
            temp = (int*)realloc(adjvertexneighbours, sizeof(int) * adjvertexsize);
            if (temp == NULL) {
                graphdestroy(graph);
                return NULL;
            }
            graph->adjacencyarray[graph->adjacencyarray[i].neighboursarray[currentsize - 1]].neighboursarray = temp;
            graph->adjacencyarray[graph->adjacencyarray[i].neighboursarray[currentsize - 1]].neighboursarray[adjvertexsize - 1] = vertex;
        }
    }
    return graph;
}

int depthfirstsearch(FILE* file, graph_t* graph) {
    if (graph == NULL)
        return 0;
    stack_t* stack = stackinit();
    if (stack == NULL) 
        return 0;
    int currentvertex = 0;
    int size = graph->size;
    if (size == 0) {
        stackdestroy(stack);
        return 0;
    }
    char* visitflag = (char*)malloc(size);
    if (visitflag == NULL) {
        stackdestroy(stack);
        return 0;
    }
    visitflag[0] = 1;
    fprintf(file, "%d ", 0);
    for (int i = 1; i < size; i++)
        visitflag[i] = 0;
    for (int i = graph->adjacencyarray[currentvertex].size - 1; i >= 0; i--)
        if (!stackpush(stack, graph->adjacencyarray[currentvertex].neighboursarray[i])) {
            free(visitflag);
            stackdestroy(stack);
            return 0;
        }
    while (!stackisempty(stack)) {
        currentvertex = stack->top->data;
        stackpop(stack);
        if (!visitflag[currentvertex]) {
            fprintf(file, "%d ", currentvertex);
            visitflag[currentvertex] = 1;
            for (int i = graph->adjacencyarray[currentvertex].size - 1; i >= 0; i--)
                if (!stackpush(stack, graph->adjacencyarray[currentvertex].neighboursarray[i])) {
                    free(visitflag);
                    stackdestroy(stack);
                    return 0;
                }
        }
    }
    free(visitflag);
    stackdestroy(stack);
    return 1;
}

int lab(void) {
    graph_t* graph = readgraph(stdin);
    if (graph == NULL)
        return 1;
    if (!depthfirstsearch(stdout, graph)) {
        graphdestroy(graph);
        return 1;
    }
    graphdestroy(graph);
    return 0;
}