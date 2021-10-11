#pragma once 
#ifdef __cplusplus
extern "C" {
#endif


#include<stdio.h> 
#include<stdlib.h> 

	typedef int data_t;

	typedef struct node_t {
		data_t data;
		struct node_t* next;
	}node_t;

	typedef struct list_t {
		node_t* head;
		node_t* tail;
	}list_t;

	typedef struct array_t {
		data_t* data;
		size_t size;
		size_t head, tail;
	}array_t;

	list_t* list_create(void);
	void list_destroy(list_t* list);
	int list_is_empty(list_t* list);
	int list_push(data_t data, list_t* list);
	int list_pop(list_t* list);
	data_t* list_front(list_t* list);
	data_t* list_back(list_t* list);

	array_t* array_create(void);
	void array_destroy(array_t* array);
	int array_is_empty(array_t* array);
	int array_push(data_t data, array_t* array);
	int array_pop(array_t* array);
	data_t* array_front(array_t* array);
	data_t* array_back(array_t* array);

#ifdef __cplusplus
}
#endif