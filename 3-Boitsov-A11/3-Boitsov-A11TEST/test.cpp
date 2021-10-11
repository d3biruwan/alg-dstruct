#include "pch.h"
#include<gtest/gtest.h>
#include"Header.h"

TEST(list_createTEST, list_create_) {
	list_t* list = list_create();
	EXPECT_NE(nullptr, list);
	free(list);
}

TEST(list_is_emptyTEST, list_is_empty_emptylist) {
	list_t list = { NULL,NULL };
	EXPECT_EQ(1, list_is_empty(&list));
}

TEST(list_is_emptyTEST, list_is_empty_nullptr) {
	list_t* list = NULL;
	EXPECT_EQ(1, list_is_empty(list));
}


TEST(list_is_emptyTEST, list_is_empty_normallist) {
	node_t tail = { 3,NULL };
	node_t element = { 2,&tail };
	node_t head = { 1,&element };
	list_t list = { &head,&tail };
	EXPECT_EQ(0, list_is_empty(&list));
}

TEST(list_pushTEST, list_push_NULLlist) {
	list_t* list = NULL;
	data_t data = 1;
	EXPECT_EQ(0, list_push(data, list));
}

TEST(list_pushTEST, list_push_Emptylist) {
	list_t list = { NULL,NULL };
	data_t data = 1;
	EXPECT_EQ(1, list_push(data, &list));
	EXPECT_EQ(1, list.head->data);
	EXPECT_EQ(nullptr, list.head->next);
	EXPECT_EQ(list.head, list.tail);
	free(list.head);
}

TEST(list_pushTEST, list_push_Normallist) {
	data_t data = 4;
	node_t tail = { 3,NULL };
	node_t element = { 2,&tail };
	node_t head = { 1,&element };
	list_t list = { &head,&tail };
	EXPECT_EQ(1, list_push(data, &list));
	EXPECT_EQ(data, list.tail->data);
	EXPECT_EQ(nullptr, list.tail->next);
	free(list.tail);
}

TEST(list_popTEST, list_pop_NULLlist) {
	list_t* list = NULL;
	EXPECT_EQ(0, list_pop(list));
}

TEST(list_popTEST, list_pop_Emptylist) {
	list_t list = { NULL,NULL };
	EXPECT_EQ(0, list_pop(&list));
}

TEST(list_popTEST, list_pop_Normallist) {
	list_t* list = (list_t*)malloc(sizeof(list_t));
	ASSERT_NE(nullptr, list);
	node_t* head = (node_t*)malloc(sizeof(node_t));
	ASSERT_NE(nullptr, head);
	node_t* tail = (node_t*)malloc(sizeof(node_t));
	ASSERT_NE(nullptr, tail);
	head->next = tail;
	head->data = 1;
	tail->next = NULL;
	tail->data = 2;
	list->head = head;
	list->tail = tail;
	EXPECT_EQ(1, list_pop(list));
	EXPECT_EQ(2, list->head->data);
	EXPECT_EQ(nullptr, list->head->next);
	free(list->head);
	free(list);
}

TEST(list_frontTEST, list_front_NULLlist) {
	list_t* list = NULL;
	EXPECT_EQ(nullptr, list_front(list));
}

TEST(list_frontTEST, list_front_Emptylist) {
	list_t list = { NULL,NULL };
	EXPECT_EQ(nullptr, list_front(&list));
}

TEST(list_frontTEST, list_front_Normallist) {
	node_t tail = { 3,NULL };
	node_t element = { 2,&tail };
	node_t head = { 1,&element };
	list_t list = { &head,&tail };
	EXPECT_EQ(1, *list_front(&list));
}

TEST(list_backTEST, list_back_NULLlist) {
	list_t* list = NULL;
	EXPECT_EQ(nullptr, list_back(list));
}

TEST(list_backTEST, list_back_Emptylist) {
	list_t list = { NULL,NULL };
	EXPECT_EQ(nullptr, list_back(&list));
}

TEST(list_backTEST, list_back_Normallist) {
	node_t tail = { 3,NULL };
	node_t element = { 2,&tail };
	node_t head = { 1,&element };
	list_t list = { &head,&tail };
	EXPECT_EQ(3, *list_back(&list));
}

TEST(array_createTEST, array_create_) {
	array_t* arr = array_create();
	EXPECT_NE(nullptr, arr);
	free(arr->data);
	free(arr);
}

TEST(array_is_emptyTEST, array_is_empty_NULLarr) {
	array_t* arr = NULL;
	EXPECT_EQ(1, array_is_empty(arr));
}

TEST(array_is_emptyTEST, array_is_empty_Emptyarr) {
	data_t data[1];
	array_t arr = { data,1,0,0 };
	EXPECT_EQ(1, array_is_empty(&arr));
}

TEST(array_is_emptyTEST, array_is_empty_Normalarr) {
	data_t data[] = { 1,2,3 };
	array_t arr = { data,3,0,3 };
	EXPECT_EQ(0, array_is_empty(&arr));
}

TEST(array_pushTEST, array_push_NULLlist) {
	array_t* arr = NULL;
	data_t data = 1;
	EXPECT_EQ(0, array_push(data, arr));
}

TEST(array_pushTEST, array_push_Emptylsit) {
	data_t* data = (data_t*)malloc(sizeof(data_t));
	ASSERT_NE(nullptr, data);
	data_t elem = 1;
	array_t arr = { data,1,0,0 };
	EXPECT_EQ(1, array_push(elem, &arr));
	EXPECT_EQ(1, arr.tail);
	EXPECT_EQ(elem, arr.data[0]);
	free(data);
}

TEST(array_pushTEST, array_push_Normallist) {
	data_t* data = (data_t*)malloc(3 * sizeof(data_t));
	ASSERT_NE(nullptr, data);
	data[0] = 1;
	data[1] = 2;
	data[2] = 3;
	data_t elem = 4;
	array_t arr = { data,3,0,3 };
	EXPECT_EQ(1, array_push(elem, &arr));
	EXPECT_EQ(4, arr.size);
	EXPECT_EQ(4, arr.tail);
	EXPECT_EQ(4, arr.data[3]);
	free(arr.data);
}

TEST(array_popTEST, array_pop_NULLlist) {
	array_t* arr = NULL;
	EXPECT_EQ(0, array_pop(arr));
}

TEST(array_popTEST, array_pop_Emptylist) {
	data_t data[1];
	array_t arr = { data,1,0,0 };
	EXPECT_EQ(0, array_pop(&arr));
}

TEST(array_popTEST, array_pop_Normallist) {
	data_t* data = (data_t*)malloc(3 * sizeof(data_t));
	ASSERT_NE(nullptr, data);
	data[0] = 1;
	data[1] = 2;
	data[2] = 3;
	array_t arr = { data,3,0,3 };
	EXPECT_EQ(1, array_pop(&arr));
	EXPECT_EQ(2, arr.size);
	EXPECT_EQ(1, arr.head);
	EXPECT_EQ(0, arr.data[0]);
	free(arr.data);
}

TEST(array_frontTEST, array_front_NULLlist) {
	array_t* arr = NULL;
	EXPECT_EQ(nullptr, array_front(arr));
}

TEST(array_frontTEST, array_front_Emptylist) {
	data_t data[1];
	array_t arr = { data,1,0,0 };
	EXPECT_EQ(nullptr, array_front(&arr));
}

TEST(array_frontTEST, array_front_Normallist) {
	data_t data[] = { 1,2,3 };
	array_t arr = { data,3,0,3 };
	EXPECT_EQ(1, *array_front(&arr));
}

TEST(array_backTEST, array_back_NULLlist) {
	array_t* arr = NULL;
	EXPECT_EQ(nullptr, array_back(arr));
}

TEST(array_backTEST, array_back_Emptylist) {
	data_t data[1];
	array_t arr = { data,1,0,0 };
	EXPECT_EQ(nullptr, array_back(&arr));
}

TEST(array_backTEST, array_back_Normallist) {
	data_t data[] = { 1,2,3 };
	array_t arr = { data,3,0,3 };
	EXPECT_EQ(3, *array_back(&arr));
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}