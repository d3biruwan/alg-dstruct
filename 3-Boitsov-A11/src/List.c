#include "Header.h"

list_t* list_create(void)
{
	list_t* list = (list_t*)malloc(sizeof(list_t));
	if (list == NULL)
		return NULL;
	else {
		list->head = NULL;
		list->tail = NULL;
		return list;
	}
}

void list_destroy(list_t* list) {
	if (!list_is_empty(list))
		while (list_pop(list));
	if(list != NULL)
		free(list);
}

int list_is_empty(list_t* list) {
	if ((list == NULL) || (((list->head) == NULL) && ((list->tail) == NULL)))
		return 1;
	else
		return 0;
}

int list_push(data_t data, list_t* list) {
	if (list == NULL)
		return 0;
	node_t* node = (node_t*)malloc(sizeof(node_t));
	if (node == NULL) 
		return 0;
	node->data = data;
	node->next = NULL;
	if (list_is_empty(list)) {
		list->head = node;
		list->tail = node;
	}
	else {
		list->tail->next = node;
		list->tail = node;
	}
	return 1;
}

int list_pop(list_t* list) {
	if (list_is_empty(list))
		return 0;
	else {
		node_t* temp = list->head;
		list->head = list->head->next;
		if (temp == list->tail) {
			free(temp);
			list->head = NULL;
			list->tail = NULL;
		}
		else
			free(temp);
		return 1;
	}
}

data_t* list_front(list_t* list) {
	if (list_is_empty(list))
		return NULL;
	return &(list->head->data);
}

data_t* list_back(list_t* list) {
	if (list_is_empty(list))
		return NULL;
	return&(list->tail->data);
}
