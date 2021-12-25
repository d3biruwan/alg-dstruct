#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>



	typedef struct {
		int id;
		int size;
		int* vertex_array;
	} colour_t;

	typedef struct {
		int position;
		int degree;
		int colour;
		int colours_number;
		int* neighbours;
		int* banned_colours;
	} vertex_t;

	int read_graph(vertex_t** priority_list, colour_t** colours, int* size, int* k, FILE* file);
	void vertex_sort(vertex_t** priority_list, vertex_t vertex);
	void colours_sort(colour_t** colours, colour_t colour);
	void initial_sort(vertex_t** priority_list, int size);
	void destroy_priority_list(vertex_t** priority_list, int size); \
		int* graph_coloring(vertex_t** priority_list, colour_t** colours, int size, int k);
	int lab_solution(FILE* input_file, FILE* output_file);

#ifdef __cplusplus
}
#endif