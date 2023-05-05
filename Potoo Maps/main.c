#include "settings.h"
#include "cJSON.h"
#include "graph.h"
#include "parsing.h"
#include "map.h"
#include "ShortestPath.h"
#include <stdlib.h>
#include <stdio.h>

#define CHARGEMENT 80

//C'est plus rapide de faire un quadtree mais c'est rapide en dictionnaire quand meme
//La difference est negligeable



int main()
{
	//Cr�ation du graphe
	//   -Tri des diff�rents points :
	int count = 0;
	int* occurrences = (int*)calloc(10000, sizeof(int));
	Point* coordinates = (Point*)calloc(10000, sizeof(Point));
	Point startingPoint, endingPoint;
	FILE* file = fopen("../Potoo Maps/esiea.geojson", "r");
	FILE* output = fopen("../Potoo Maps/output.geojson","w");
	
	count = parsingFile(file, coordinates);

	Graph* graph = Graph_create(count);
	for (int i = 0; i < count; i++)
	{
		Graph_setCoordinates(graph, i, coordinates[i]);
	}
	graphMap(graph, count, coordinates, file);

	//Graph_print(graph);

	Point* route = (Point*)calloc(count, sizeof(Point));
	inputCoordinates(coordinates, count, &startingPoint, &endingPoint);
	printf("%lf, %lf\n %lf, %lf\n", startingPoint.longitude, startingPoint.latitude, endingPoint.longitude, endingPoint.latitude);


	//Liberation de memoire
	free(coordinates);
	free(occurrences);
	free(route);
	fclose(file);
	fclose(output);
	Graph_destroy(graph);
	//size = findPath(graph, 15, 356, route);

	//writeOutput(route, size);

	/*//Recherche du plus court chemin
	Path* path = Graph_shortestPath(graph, 148, 14);
	IntListNode* current = path->list->sentinel.next;
	Point* res = (Point*)calloc(path->list->nodeCount, sizeof(Point));
	int k = 0;
	while (current != &path->list->sentinel)
	{
		res[k++] = Graph_getCoordinates(graph, current->value);
		current = current->next;
	}*/

	/*//Ecriture du fichier de sortie
	FILE* test = fopen("../output.json", "w");
	char* bFile = "{\n		\"type\": \"FeatureCollection\",\n		\"features\" : [\n	{\n		\"type\": \"Feature\",\n			\"geometry\" :\n		{\n			\"type\": \"LineString\",\n				\"coordinates\" : [\n";
	char* eFile = "				]\n			}\n		}\n	]\n}\n";

	fprintf(test, bFile);
	for (int i = 0; i < k; i++)
	{
		if(i+1 == k)
			fprintf(test, "					[%lf, %lf]\n", res[i].longitude, res[i].latitude);
		else
			fprintf(test, "					[%lf, %lf],\n", res[i].longitude, res[i].latitude);
	}
	fprintf(test, eFile);*/
}