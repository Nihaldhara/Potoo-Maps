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
	int* occurrences = (int*)calloc(10000, sizeof(int));
	Point* coordinates = (Point*)calloc(10000, sizeof(Point));
	Point startingPoint, endingPoint;

#ifdef PRETRAITEMENT
	Graph* graph = readTraitement(&count, coordinates);
#endif // PRETRAITEMENT

#ifndef PRETRAITEMENT
	FILE* file = fopen("../Potoo Maps/esiea.geojson", "r");
	count = parsingFile(file, coordinates);
	Graph* graph = Graph_create(count);
	for (int i = 0; i < count; i++)
	{
		Graph_setCoordinates(graph, i, coordinates[i]);
	}

	graphMap(graph, count, coordinates, file);
	writeTraitement(graph, count);

#endif // !PRETRAITEMENT


	//Graph_print(graph);
	
	Point* route = (Point*)calloc(count, sizeof(Point));
	inputCoordinates(coordinates, count, &startingPoint, &endingPoint, &idStart, &idEnd);
	size = findPath(graph, idStart, idEnd, route);

	//inputCoordinates(coordinates, count, &startingPoint, &endingPoint);
	printf("%lf, %lf\n %lf, %lf\n", startingPoint.longitude, startingPoint.latitude, endingPoint.longitude, endingPoint.latitude);


	//Liberation de memoire
	free(coordinates);
	free(occurrences);

#ifndef PRETRAITEMENT
	fclose(file);
#endif

	Graph_destroy(graph);

	writeOutput(route, size);
	free(route);
}