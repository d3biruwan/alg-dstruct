/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10  Ver. 20H2 Build 19042.1052
PC configuration:
	CPU:			Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz   2.20 GHz
	Motherboard:    Hewlett-Packard 2248
	RAM:            8.00 GB
	HDD:            222 GB
	Time: 40.4 sec (test) 53.5 sec (total time)
	Used memory: around 702 MB*/

#include "pch.h"
#include<gtest/gtest.h>
#include"Header.h"
#define NUMBER_OF_NODES 4000;

TEST(stress_test, graphinit) {
	int size = NUMBER_OF_NODES;
	FILE* file = fopen("graph.txt", "w");
	ASSERT_NE(nullptr, file);
	fprintf(file, "%d\n", size);
	for (int i = 0; i < size; i++) {
		fprintf(file, "%d", i);
		for (int j= i+1; j < size; j++) 
			fprintf(file, " %d", j);	
		fprintf(file, "\n");
	}
	fclose(file);
}

TEST(stress_test, test) {
	graph_t* graph = NULL;
	FILE* inputfile = fopen("graph.txt", "r");
	ASSERT_NE(nullptr, inputfile);
	graph = readgraph(inputfile);
	ASSERT_NE(nullptr, graph);
	fclose(inputfile);
	FILE* outputfile = fopen("res.txt", "w");
	if (outputfile == NULL) {
		graphdestroy(graph);
		ASSERT_NE(nullptr, outputfile);
	}
	int result = depthfirstsearch(outputfile, graph);
	if (!result) {
		fclose(outputfile);
		graphdestroy(graph);
		ASSERT_EQ(1, result);
	}
	fclose(outputfile);
	graphdestroy(graph);
}
