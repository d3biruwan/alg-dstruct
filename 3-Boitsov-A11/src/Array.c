#include"Header.h"

array_t* array_create(void) {
	array_t* arr = (array_t*)malloc(sizeof(array_t));
	if (arr == NULL)
		return NULL;
	data_t* data = (data_t*)malloc(sizeof(data_t));
	if (data == NULL) {
		free(arr);
		return NULL;
	}
	arr->data = data;
	arr->head = 0;
	arr->tail = 0;
	arr->size = 1;
	return arr;
}

int array_is_empty(array_t* array) {
	if ((array == NULL) || (array->head == array->tail))
		return 1;
	else
		return 0;
}

int array_push(data_t data, array_t* array) {
	if (array == NULL)
		return 0;
	if (array->tail < array->size)
		array->data[array->tail++] = data;
	else {
		(array->size)++;
		data_t* temp = (data_t*)realloc(array->data, (array->size) * sizeof(data_t));
		if (temp == NULL) {
			(array->size)--;
			return 0;
		}
		array->data = temp;
		array->data[array->tail++] = data;
	}
	return 1;
}

int array_pop(array_t* array) {
	if (array_is_empty(array))
		return 0;
	array->data[array->head++] = 0;
	array->size--;
	return 1;
}

data_t* array_front(array_t* array) {
	if (array_is_empty(array))
		return NULL;
	return &(array->data[array->head]);
}

data_t* array_back(array_t* array) {
	if (array_is_empty(array))
		return NULL;
	return &(array->data[array->tail - 1]);
}

void array_destroy(array_t* array) {
	free(array->data);
	free(array);
}