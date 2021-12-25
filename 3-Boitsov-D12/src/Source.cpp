#include"Header.h"

int read_graph(vertex_t** priority_list, colour_t** colours, int* size, int* k, FILE* file) {
	//int size,k;
	fscanf(file, "%d %d", size, k);
	vertex_t* temp1 = (*priority_list);
	temp1 = (vertex_t*)realloc(temp1, *size * sizeof(vertex_t));
	if (temp1 == NULL)
		return 0;
	(*priority_list) = temp1;
	colour_t* temp2 = (*colours);
	temp2 = (colour_t*)realloc(temp2, *k * sizeof(colour_t));
	if (temp2 == NULL) {
		free(*priority_list);
		return 0;
	}
	(*colours) = temp2;
	// filling the colours list
	for (int i = 0; i < *k; i++) {
		(*colours)[i].id = *k - i;
		(*colours)[i].size = 0;
		(*colours)[i].vertex_array = NULL;
	}
	// filling the priority list
	for (int i = 0; i < *size; i++) {
		(*priority_list)[i].position = i + 1;
		(*priority_list)[i].degree = 1;
		(*priority_list)[i].banned_colours = NULL;
		(*priority_list)[i].neighbours = NULL;
		(*priority_list)[i].colour = 0;
		(*priority_list)[i].colours_number = 0;
		int* temp = (*priority_list)[i].neighbours;
		temp = (int*)realloc(temp, sizeof(int));
		if (temp == NULL) {
			destroy_priority_list(priority_list, i + 1);
			free(*colours);
			return 0;
		}
		(*priority_list)[i].neighbours = temp;
		fscanf(file, "%d", &(*priority_list)[i].neighbours[0]);
		while ((!feof(file)) && (fgetc(file) == ' ')) {
			(*priority_list)[i].degree++;
			temp = (int*)realloc(temp, (*priority_list)[i].degree * sizeof(int));// maybe check temp
			if (temp == NULL) {
				destroy_priority_list(priority_list, i + 1);
				free(*colours);
				return 0;
			}
			(*priority_list)[i].neighbours = temp;
			fscanf(file, "%d", &(*priority_list)[i].neighbours[(*priority_list)[i].degree - 1]);
		}
	}
	initial_sort(priority_list, *size);
	return 1;
}

void vertex_sort(vertex_t** priority_list, vertex_t vertex) {
	for (int i = 0; i < vertex.degree; i++) {
		int position = vertex.neighbours[i];
		int counter = 0;
		//while ((*priority_list)[counter++].position != position);
		while ((*priority_list)[counter].position != position)
			counter++;
		if ((*priority_list)[counter].colour != 0)
			continue;
		while ((counter > 0) && ((*priority_list)[counter].colours_number > (*priority_list)[counter - 1].colours_number)) {
			if ((*priority_list)[counter - 1].colour != 0)
				break;
			vertex_t temp = (*priority_list)[counter];
			(*priority_list)[counter] = (*priority_list)[counter - 1];
			(*priority_list)[counter - 1] = temp;
			counter--;
		}
		while ((counter > 0) && ((*priority_list)[counter].colours_number == (*priority_list)[counter - 1].colours_number) && ((*priority_list)[counter].degree > (*priority_list)[counter - 1].degree)) {
			if ((*priority_list)[counter - 1].colour != 0)
				break;
			vertex_t temp = (*priority_list)[counter];
			(*priority_list)[counter] = (*priority_list)[counter - 1];
			(*priority_list)[counter - 1] = temp;
			counter--;
		}
	}
}

void colours_sort(colour_t** colours, colour_t colour) {
	int id = colour.id;
	int counter = 0;
	//while ((*colours)[counter++].id != id);
	while ((*colours)[counter].id != id)
		counter++;
	while ((counter > 0) && ((*colours)[counter].size > (*colours)[counter - 1].size)) {
		colour_t temp = (*colours)[counter];
		(*colours)[counter] = (*colours)[counter - 1];
		(*colours)[counter - 1] = temp;
	}
}

void initial_sort(vertex_t** priority_list, int size) {
	for (int i = size - 1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if ((*priority_list)[j].degree < (*priority_list)[j + 1].degree) {
				vertex_t temp = (*priority_list)[j];
				(*priority_list)[j] = (*priority_list)[j + 1];
				(*priority_list)[j + 1] = temp;
			}
		}
	}
}

void destroy_priority_list(vertex_t** priority_list, int size) {
	for (int i = 0; i < size; i++) {
		free((*priority_list)[i].banned_colours);
		free((*priority_list)[i].neighbours);
	}
	free(*priority_list);
}

void destroy_colours_array(colour_t** colours, int size) {
	for (int i = 0; i < size; i++)
		free((*colours)[i].vertex_array);
	free(*colours);
}

int* graph_coloring(vertex_t** priority_list, colour_t** colours, int size, int k) {
	int counter = 0;
	while (counter != size) {
		//vertex_t current_vertex = priority_list[counter];
		for (int i = k - 1; i >= 0; i--) {
			int sufficent_colour = 1;
			for (int j = 0; j < (*priority_list)[counter].colours_number; j++)
				if ((*priority_list)[counter].banned_colours[j] == (*colours)[i].id) {
					sufficent_colour = 0;
					break;
				}
			if (sufficent_colour) {
				(*priority_list)[counter].colour = (*colours)[i].id;
				for (int j = 0; j < (*priority_list)[counter].degree; j++) {
					int position = (*priority_list)[counter].neighbours[j];
					int list_position = 0;
					//while ((*priority_list)[list_position++].position != position);
					while ((*priority_list)[list_position].position != position)
						list_position++;
					if ((*priority_list)[list_position].colour != 0)
						continue;
					(*priority_list)[list_position].colours_number++;
					int* temp = (*priority_list)[list_position].banned_colours;
					temp = (int*)realloc(temp, (*priority_list)[list_position].colours_number * sizeof(int));
					if (temp == NULL) {
						destroy_priority_list(priority_list, size);
						destroy_colours_array(colours, k);
						return NULL;
					}
					(*priority_list)[list_position].banned_colours = temp;
					(*priority_list)[list_position].banned_colours[(*priority_list)[list_position].colours_number - 1] = (*priority_list)[counter].colour;
				}
				(*colours)[i].size++;
				int* c_temp = (*colours)[i].vertex_array;
				c_temp = (int*)realloc(c_temp, sizeof(int) * (*colours)[i].size);
				if (c_temp == NULL) {
					destroy_priority_list(priority_list, size);
					destroy_colours_array(colours, k);
					return NULL;
				}
				(*colours)[i].vertex_array = c_temp;
				(*colours)[i].vertex_array[(*colours)[i].size - 1] = (*priority_list)[counter].position;
				vertex_sort(priority_list, (*priority_list)[counter]);
				colours_sort(colours, (*colours)[i]);
				break;
			}
		}
		if ((*priority_list)[counter].colour == 0) {
			destroy_priority_list(priority_list, size);
			destroy_colours_array(colours, k);
			return NULL;
		}
		counter++;
	}
	int* coloring = NULL;
	coloring = (int*)malloc(size * sizeof(int));
	if (coloring == NULL) {
		destroy_priority_list(priority_list, size);
		destroy_colours_array(colours, k);
		return NULL;
	}
	for (int i = 0; i < size; i++)
		coloring[(*priority_list)[i].position - 1] = (*priority_list)[i].colour;
	return coloring;
}

int lab_solution(FILE* input_file, FILE* output_file) {
	int size, k;
	vertex_t* priority_list = NULL;
	colour_t* colours = NULL;
	int read = read_graph(&priority_list, &colours, &size, &k, input_file);
	if (read == 0) {
		fprintf(output_file, "Unable to read the graph\n");
		return 0;
	}
	int* coloring = graph_coloring(&priority_list, &colours, size, k);
	if (coloring == NULL) {
		fprintf(output_file, "0");
		return 0;
	}
	for (int i = 0; i < size; i++)
		fprintf(output_file, "%d ", coloring[i]);
	destroy_priority_list(&priority_list, size);
	destroy_colours_array(&colours, k);
	return 1;
}

//int main(void) {
//	lab_solution(stdin, stdout);
//	return 0;
//}