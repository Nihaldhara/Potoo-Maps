#include "settings.h"
#include "cJSON.h"
#include "graph.h"
#include "parsing.h"
#include "map.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	//Création du graphe
	//   -Tri des différents points :
	int count = 0;
	int* occurrences = (int*)calloc(10000, sizeof(int));
	Point* stockNodes = (Point*)calloc(10000, sizeof(Point));
	FILE* file = fopen("../Potoo Maps/esiea.geojson", "r");
	char* segment = calloc(4096, sizeof(char));

	//Calcul de la longueur maximale du fichier et initialisation de la barre de chargement
	long total, atm, previous = 0;
	fseek(file, 0, SEEK_END);
	total = ftell(file);
	rewind(file);
	(float)total /= 20.0f;
	printf("Lecture du fichier\n-------------------\n"); // La barre de chargement doit etre égal a n-1, n était le diviseur de "total"

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

	//(Affichage)
	printf("\n");
	for (int i = 0; i < count; i++)
	{
		if (occurrences[i] > 1)
		{
			printf("%d | %lf %lf\n", occurrences[i], stockNodes[i].latitude, stockNodes[i].longitude);
		}
	}

	/*//Création du graphe
	Graph* graph = Graph_create(count);
	for (int i = 0; i < count; i++)	Graph_setCoordinates(graph, i, stockNodes[i]);
	Graph_print(graph);

	rewind(file);
	for (int i = 0; i < 156; i++)
		fgets(segment, 4096, file);

	while (!feof(file))
	{
		fgets(segment, 4096, file);
		cJSON* object = cJSON_ParseWithLength(segment, 4096);
		cJSON* jNodes = cJSON_GetObjectItem(object, "nodes");
		if (cJSON_IsArray(jNodes) == true)
		{
			cJSON* jNode = NULL;
			bool encountered = false;
			double distance = 0.0f;
			int lastIntersection = 0;
			cJSON_ArrayForEach(jNode, jNodes)
			{
				cJSON* jLat = cJSON_GetObjectItem(jNode, "lat");
				cJSON* jLon = cJSON_GetObjectItem(jNode, "lon");
				
				for (int i = 0; i < count-1; i++)
				{
					Point current = Graph_getCoordinates(graph, i);
					Point next = Graph_getCoordinates(graph, i + 1);
					if (!encountered
						&& atof(cJSON_GetStringValue(jLat)) == current.latitude
						&& atof(cJSON_GetStringValue(jLon)) == current.longitude)
					{
						encountered = true;
						lastIntersection = i;
					}

					else if (encountered
						&& atof(cJSON_GetStringValue(jLat)) == current.latitude
						&& atof(cJSON_GetStringValue(jLon)) == current.longitude)
					{
						//printf("%lf\n", distance);
						//printf("\n-----------------------------------------------------------------\n");
						Graph_set(graph, lastIntersection, i, distance);
						distance = 0.0f;
					}

					else
					{
						distance += Distance(current, next);
					}
				}
			}
		}
	}
	*/
	//Graph_print(graph);
}