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
	FILE* file = fopen("../Potoo Maps/france.geojson", "r");
	Point* coordinates = parsingFile(file, &count);

#ifdef PRETRAITEMENT
	Graph* graph = readTraitement();
#endif // PRETRAITEMENT

#ifndef PRETRAITEMENT
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
	
	fclose(file);

	Graph_destroy(graph);
	//size = findPath(graph, 15, 356, route);

	writeOutput(route, size);
	free(route);
}