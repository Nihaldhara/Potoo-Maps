#include "settings.h"
#include "cJSON.h"
#include "graph.h"
#include "parsing.h"
#include "map.h"
#include "ShortestPath.h"
#include <stdlib.h>
#include <stdio.h>

#define CHARGEMENT 80

int main()
{
	//Création du graphe
	//   -Tri des différents points :
	int count = 0;

	int* occurrences = (int*)calloc(10000, sizeof(int));
	Point* stockNodes = (Point*)calloc(10000, sizeof(Point));
	FILE* file = fopen("../Potoo Maps/esiea.geojson", "r");
	FILE* output = fopen("../Potoo Maps/output.geojson","w");
	char* segment = calloc(4096, sizeof(char));
	for (int i = 0; i < 156; i++)
		fgets(segment, 4096, file);

	//Calcul de la longueur maximale du fichier et initialisation de la barre de chargement
	long total, atm, previous = 0;
	fseek(file, 0, SEEK_END);
	total = ftell(file);
	rewind(file);
	(float)total /= CHARGEMENT;
	printf("Lecture du fichier en cours\n"); // La barre de chargement doit etre égal a n-1, n était le diviseur de "total"
	for (int i = 0; i < CHARGEMENT-1; i++)
		printf("-");
	printf("\n");

	//Boucle de lecture et stockage des différentes intersections
	while(!feof(file))
	{
		fgets(segment, 4096, file);
		cJSON* object = cJSON_ParseWithLength(segment, 4096);
		cJSON* jType = cJSON_GetObjectItem(object, "type");

		if (strcmp(cJSON_GetStringValue(jType), "way") != 0)
		{
			continue;
		}

		//Calcul du pourcentage d'avancement de la barre de chargement
		atm = ftell(file);
		float res = (float)atm / (float)total;
		if ((float)atm >= ((float)previous + (float)total))
		{
			previous = atm;
			printf("#");
		}
		
		cJSON* jNodes = cJSON_GetObjectItem(object, "nodes");
		if (cJSON_IsArray(jNodes) == true)
		{
			cJSON* jNode = NULL;
			cJSON_ArrayForEach(jNode, jNodes)
			{
				cJSON* jLat = cJSON_GetObjectItem(jNode, "lat");
				cJSON* jLon = cJSON_GetObjectItem(jNode, "lon");

				double dLat = atof(cJSON_GetStringValue(jLat));
				double dLon = atof(cJSON_GetStringValue(jLon));

				bool found = false;
				for(int j = 0; j < count; j++)
				{
					if (dLat == stockNodes[j].latitude
						&& dLon == stockNodes[j].longitude)
					{
						occurrences[j]++;
						found = true;
						break;
					}
				}

				if (!found)
				{
					stockNodes[count].latitude = dLat;
					stockNodes[count].longitude = dLon;
					occurrences[count++]++;
				}
			}
		}
	}

	//Rectifications parce que je sais pas coder proprement
	int* newOccurrences = (int*)calloc(count, sizeof(int));
	Point* coordinates = (Point*)calloc(count, sizeof(Point));
	int realCount = 0;
	printf("\n");
	for (int i = 0; i < count; i++)
	{
		if (occurrences[i] > 1)
		{
			//printf("%d | %lf %lf\n", occurrences[i], stockNodes[i].latitude, stockNodes[i].longitude);
			newOccurrences[realCount] = occurrences[i];
			coordinates[realCount++] = stockNodes[i];
		}
	}

	//Création du graphe
	Graph* graph = Graph_create(realCount);
	for (int i = 0; i < realCount; i++)
	{
		Graph_setCoordinates(graph, i, coordinates[i]);
	}

	rewind(file);
	while (!feof(file))
	{
		fgets(segment, 4096, file);
		cJSON* object = cJSON_ParseWithLength(segment, 4096);
		cJSON* jType = cJSON_GetObjectItem(object, "type");

		if (strcmp(cJSON_GetStringValue(jType), "way") != 0)
		{
			continue;
		}

		cJSON* jNodes = cJSON_GetObjectItem(object, "nodes");
		Point lastCoordinates, currentCoordinates;
		lastCoordinates.latitude = 0;
		lastCoordinates.longitude = 0;
		bool encountered = false;
		double distance = 0.0f;
		int lastIntersection = 0;
		if (cJSON_IsArray(jNodes) == true)
		{
			cJSON* jNode = NULL;
			cJSON_ArrayForEach(jNode, jNodes)
			{
				cJSON* jLat = cJSON_GetObjectItem(jNode, "lat");
				cJSON* jLon = cJSON_GetObjectItem(jNode, "lon");

				double dLat = atof(cJSON_GetStringValue(jLat));
				double dLon = atof(cJSON_GetStringValue(jLon));

				currentCoordinates.latitude = dLat;
				currentCoordinates.longitude = dLon;

				if (encountered)
					distance += Distance(lastCoordinates, currentCoordinates);

				for (int i = 0; i < count; i++)
				{
					if (encountered
						&& coordinates[i].latitude == dLat
						&& coordinates[i].longitude == dLon)
					{
						Graph_set(graph, lastIntersection, i, distance);
						Graph_set(graph, i, lastIntersection, distance);
						distance = 0.0f;
						encountered = false;
					}

					if (!encountered
						&& coordinates[i].latitude == dLat
						&& coordinates[i].longitude == dLon)
					{
						encountered = true;
						lastIntersection = i;
					}
				}

				lastCoordinates.latitude = dLat;
				lastCoordinates.longitude = dLon;
			}
		}
	}

	Graph_print(graph);

	//Recherche du plus court chemin
	Path* path = Graph_shortestPath(graph, 148, 14);
	IntListNode* current = path->list->sentinel.next;
	Point* res = (Point*)calloc(path->list->nodeCount, sizeof(Point));
	int k = 0;
	while (current != &path->list->sentinel)
	{
		res[k++] = Graph_getCoordinates(graph, current->value);
		current = current->next;
	}

	//Ecriture du fichier de sortie
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
	fprintf(test, eFile);
}