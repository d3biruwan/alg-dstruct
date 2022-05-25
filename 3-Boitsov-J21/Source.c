#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TABLE_SIZE 1000
#define STEP 7
#define BUFFER_SIZE 50


typedef struct node_t {
	char* str;
	int hash_val, is_free;
} node_t;


typedef struct table_t {
	node_t* data;
	int size;
	int step;
} table_t;


table_t create_table(const size_t size, const size_t step) {
	table_t res;
	res.step = step;
	res.data = (node_t*)malloc(size * sizeof(node_t));
	res.size = res.data ? size : 0;
	for (int i = 0; i < res.size; i++) {
		res.data[i].str = NULL;
		res.data[i].is_free = 1;
		res.data[i].hash_val = -1
			;
	}
	return res;
}

void destroy_table(table_t* hash_table) {
	for (int i = 0; i < hash_table->size; i++)
		if (hash_table->data[i].str)
			free(hash_table->data[i].str);
	free(hash_table->data);
	hash_table->size = 0;
	hash_table->data = NULL;
}


int hash_function(const char* str, int table_size) {
	int hash = 0;
	for (int i = 0; str[i] != '\0'; i++)
		hash = hash * 16 + str[i];
	return hash % table_size;
}


int hash_find(const char* str, table_t* hash_table, int index_start) {
	int free_index = -1;
	int index_cur = index_start;
	while ((!hash_table->data[index_cur].is_free) || ((hash_table->data[index_cur].hash_val != index_start) && (hash_table->data[index_cur].hash_val != -1))) {
		if (hash_table->data[index_cur].hash_val != index_start) {
			index_cur = (index_cur + hash_table->step) % hash_table->size;
			if ((hash_table->data[index_cur].is_free) && (free_index == -1))
				free_index = index_cur;
			continue;
		}
		if (!strcmp(str, hash_table->data[index_cur].str))
			return index_cur;
		index_cur = (index_cur + hash_table->step) % hash_table->size;
		if ((index_cur == index_start) && (free_index == -1))
			return -1;
		if (index_cur == index_start)
			return free_index;
	}
	return index_cur;
}


int find_element(const char* str, table_t* hash_table) {
	int hash = hash_function(str, hash_table->size);
	int index = hash_find(str, hash_table, hash);
	if (index == -1)
		return 0;
	return hash_table->data[index].is_free ? 0 : index;
}


int hash_insert(const char* str, table_t* hash_table) {
	int hash = hash_function(str, hash_table->size);
	int index = hash_find(str, hash_table, hash);
	if (index == -1)
		return 0;
	if (hash_table->data[index].is_free) {
		hash_table->data[index].is_free = 0;
		hash_table->data[index].hash_val = hash;
		hash_table->data[index].str = (char*)malloc(sizeof(char) * (strlen(str) + 1));
		if (hash_table->data[index].str == NULL)
			return 0;
		strcpy(hash_table->data[index].str, str);
		hash_table->data[index].is_free = 0;
		hash_table->data[index].hash_val = hash;
		return index;
	}
	else
		return 0;
}


int hash_remove(const char* str, table_t* hash_table) {
	int index_start = hash_function(str, hash_table->size);
	int index = hash_find(str, hash_table, index_start);
	if ((index == -1) || (hash_table->data[index].is_free))
		return 0;
	int index_next = (index + hash_table->step) % hash_table->size;
	while (!(hash_table->data[index_next].is_free) && (index_next != index_start)) {
		if (hash_table->data[index_next].hash_val != index_start) {
			index_next = (index_next + hash_table->step) % hash_table->size;
			continue;
		}
		hash_table->data[index].str = hash_table->data[index_next].str;
		index_next = (index_next + hash_table->step) % hash_table->size;
	}
	hash_table->data[index].is_free = 1;
	free(hash_table->data[index].str);
	hash_table->data[index].str = NULL;
	return 1;
}


int test(table_t* table) {
	char buffer[BUFFER_SIZE] = { 0 };
	char c = 0;
	char string[BUFFER_SIZE] = { 0 };
	while (fgets(buffer, BUFFER_SIZE, stdin)) {
		sscanf(buffer, "%c %s", &c, &string);
		switch (c) {
		case 'a':
			hash_insert(string, table);
			break;
		case 'r':
			hash_remove(string, table);
			break;
		case 'f':
			printf("%s\n", find_element(string, table) ? "yes" : "no");
			break;
		default:
			destroy_table(table);
			return 0;
		}
	}
	destroy_table(table);
	return 0;
}


int main(void) {
	table_t table = create_table(TABLE_SIZE, STEP);
	test(&table);
	return 0;
}

