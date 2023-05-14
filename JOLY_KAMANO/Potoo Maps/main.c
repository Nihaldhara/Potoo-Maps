#include "settings.h"
#include "cJSON.h"
#include "graph.h"
#include "parsing.h"
#include "map.h"
#include "ShortestPath.h"
#include <stdlib.h>
#include <stdio.h>

//#define PRETRAITEMENT 

int main()
{
	int count = 0, size = 0, idStart = 0, idEnd = 0;
	Point startingPoint, endingPoint;

#ifdef PRETRAITEMENT
	Graph* graph = readTraitement(&count, coordinates);
#endif // PRETRAITEMENT

#ifndef PRETRAITEMENT
	FILE* file = fopen("../../Data/mayenne.geojson", "r");
	Point* coordinates = parsingFile(file, &count);
	Graph* graph = Graph_create(count);
	for (int i = 0; i < count; i++)
	{
		Graph_setCoordinates(graph, i, coordinates[i]);
	}

	graphMap(graph, count, coordinates, file);
	writeTraitement(graph, count);

#endif // !PRETRAITEMENT
	
	Point* route = (Point*)calloc(count, sizeof(Point));
	inputCoordinates(coordinates, count, &startingPoint, &endingPoint, &idStart, &idEnd);
	size = findPath(graph, idStart, idEnd, route);

	//Liberation de memoire
	free(coordinates);

#ifndef PRETRAITEMENT
	fclose(file);
#endif

	Graph_destroy(graph);

	writeOutput(route, size);
	free(route);
}