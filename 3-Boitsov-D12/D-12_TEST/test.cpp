/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10  Ver. 20H2 Build 19042.1052
PC configuration:
	CPU:			Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz   2.20 GHz
	Motherboard:    Hewlett-Packard 2248
	RAM:            8.00 GB
	HDD:            222 GB
	Time: 35 sec (test) 41.5 sec (total time)
	Used memory: around 75 MB*/

#include "pch.h"
#include<gtest/gtest.h>
#include"../src/Header.h"

#define NODES 2000
#define COLOURS 2000

TEST(TestCaseName, TestName) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(stress_test, graph_init) {
	FILE* input = fopen("stress_input.txt", "w");
	ASSERT_NE(nullptr, input);
	fprintf(input, "%d %d\n", NODES, COLOURS);
	for (int j = 1; j <= 2000; j++) {
		for (int k = 1; k <= 2000; k++) {
			if ((k != 1) && ((k - 1) != j)&&!((k==2000)&&(j==2000)))
				fprintf(input, " ");
			if (k == j)
				continue;
			fprintf(input, "%d", k);
		}
		fprintf(input, "\n");
	}
	fclose(input);
}

TEST(stress_test, test) {
	FILE* input = fopen("stress_input.txt", "r");
	ASSERT_NE(nullptr, input);
	FILE* output = fopen("stress_output.txt", "w");
	ASSERT_NE(nullptr, output);
	EXPECT_EQ(1, lab_solution(input, output));
	fclose(input);
	fclose(output);
}


/* Минимальное количество цветов для раскраски определяется максимальным количеством связанных между собой узлов
(если присутсвует связанная пара, то минимум 2 цвета, если тройка-3 и т.д). В этих тестах я проверяю выполнение этого условия. */
TEST(func_tests, example_test) {
	FILE* input = fopen("input.txt", "w");
	ASSERT_NE(nullptr, input);
	fprintf(input, "3 3\n2 3\n1 3\n1 2");
	fclose(input);
	input = fopen("input.txt", "r");
	ASSERT_NE(nullptr, input);
	FILE* output = fopen("output.txt", "w");
	ASSERT_NE(nullptr, output);
	EXPECT_EQ(1, lab_solution(input, output));
	fclose(input);
	fclose(output);
	output = fopen("output.txt", "r");
	char res[100];
	fgets(res, 100, output);
	EXPECT_EQ(strcmp(res, "1 2 3 "),0);
	fclose(output);
}

TEST(func_tests, not_enough_colours) {
	FILE* input = fopen("input.txt", "w");
	ASSERT_NE(nullptr, input);
	fprintf(input, "2 1\n2\n1");
	fclose(input);
	input = fopen("input.txt", "r");
	ASSERT_NE(nullptr, input);
	FILE* output = fopen("output.txt", "w");
	ASSERT_NE(nullptr, output);
	EXPECT_EQ(0, lab_solution(input, output));
	fclose(input);
	fclose(output);
	output = fopen("output.txt", "r");
	char res[100];
	fgets(res, 100, output);
	EXPECT_EQ(strcmp(res, "0"), 0);
	fclose(output);
}

TEST(func_tests, line_graph_2_colors) {
	FILE* input = fopen("input.txt", "w");
	ASSERT_NE(nullptr, input);
	fprintf(input, "8 2\n2\n1 3\n2 4\n3 5\n4 6\n5 7\n6 8\n7");
	fclose(input);
	input = fopen("input.txt", "r");
	ASSERT_NE(nullptr, input);
	FILE* output = fopen("output.txt", "w");
	ASSERT_NE(nullptr, output);
	EXPECT_EQ(1, lab_solution(input, output));
	fclose(input);
	fclose(output);
	output = fopen("output.txt", "r");
	char res[100];
	fgets(res, 100, output);
	EXPECT_EQ(strcmp(res, "2 1 2 1 2 1 2 1 "), 0);
	fclose(output);
}

TEST(func_tests, 3_nodes_2_colors) {
	FILE* input = fopen("input.txt", "w");
	ASSERT_NE(nullptr, input);
	fprintf(input, "3 2\n2 3\n1 3\n1 2");
	fclose(input);
	input = fopen("input.txt", "r");
	ASSERT_NE(nullptr, input);
	FILE* output = fopen("output.txt", "w");
	ASSERT_NE(nullptr, output);
	EXPECT_EQ(0, lab_solution(input, output));
	fclose(input);
	fclose(output);
	output = fopen("output.txt", "r");
	char res[100];
	fgets(res, 100, output);
	EXPECT_EQ(strcmp(res, "0"), 0);
	fclose(output);
}

TEST(func_tests, 3_nodes_3_colors) {
	FILE* input = fopen("input.txt", "w");
	ASSERT_NE(nullptr, input);
	fprintf(input, "4 3\n2 3\n1 3 4\n1 2 4\n2 3");
	fclose(input);
	input = fopen("input.txt", "r");
	ASSERT_NE(nullptr, input);
	FILE* output = fopen("output.txt", "w");
	ASSERT_NE(nullptr, output);
	EXPECT_EQ(1, lab_solution(input, output));
	fclose(input);
	fclose(output);
	output = fopen("output.txt", "r");
	char res[100];
	fgets(res, 100, output);
	EXPECT_EQ(strcmp(res, "3 1 2 3 "), 0);
	fclose(output);
}

TEST(func_tests, 4_nodes_3_colors) {
	FILE* input = fopen("input.txt", "w");
	ASSERT_NE(nullptr, input);
	fprintf(input, "4 3\n2 3 4\n1 3 4\n1 2 4\n1 2 3");
	fclose(input);
	input = fopen("input.txt", "r");
	ASSERT_NE(nullptr, input);
	FILE* output = fopen("output.txt", "w");
	ASSERT_NE(nullptr, output);
	EXPECT_EQ(0, lab_solution(input, output));
	fclose(input);
	fclose(output);
	output = fopen("output.txt", "r");
	char res[100];
	fgets(res, 100, output);
	EXPECT_EQ(strcmp(res, "0"), 0);
	fclose(output);
}

TEST(func_tests, 4_nodes_4_colors) {
	FILE* input = fopen("input.txt", "w");
	ASSERT_NE(nullptr, input);
	fprintf(input, "4 4\n2 3 4\n1 3 4\n1 2 4\n1 2 3");
	fclose(input);
	input = fopen("input.txt", "r");
	ASSERT_NE(nullptr, input);
	FILE* output = fopen("output.txt", "w");
	ASSERT_NE(nullptr, output);
	EXPECT_EQ(1, lab_solution(input, output));
	fclose(input);
	fclose(output);
	output = fopen("output.txt", "r");
	char res[100];
	fgets(res, 100, output);
	EXPECT_EQ(strcmp(res, "1 2 3 4 "), 0);
	fclose(output);
}