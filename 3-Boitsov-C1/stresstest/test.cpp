#include "pch.h"
#include<gtest/gtest.h>
#include"Header.h"
#define NUMBER_OF_NODES 3000;
TEST(stress_test, graphinit) {
	int size = NUMBER_OF_NODES;
	FILE* f = fopen("graph.txt", "w");
	ASSERT_NE(nullptr, f);
	fprintf(f, "%d\n", size);
	for (int i = 0; i < size; i++) {
		fprintf(f, "%d", i);
		for (int j = 0; j < size; j++) {
			if (j != i) {
				fprintf(f, " %d", j);
			}
		}
		fprintf(f, "\n");
	}
	fclose(f);
}
TEST(stress_test, test) {
	graph* g = NULL;
	FILE* f = fopen("graph.txt", "r");
	ASSERT_NE(nullptr, f);
	g = readgraph(f);
	ASSERT_NE(nullptr, g);
	fclose(f);
	FILE* h = fopen("res.txt", "w");
	if (h == NULL) {
		graphdestroy(g);
		ASSERT_NE(nullptr, h);
	}
	int res = DFS(h, g);
	if (!res) {
		fclose(h);
		graphdestroy(g);
		ASSERT_EQ(1, res);
	}
	fclose(h);
	graphdestroy(g);
}
