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
	for (int i = 0; i < 156; i++)
		fgets(segment, 4096, file);

	//Boucle de lecture et stockage des différentes intersections
	while(!feof(file))
	{
		fgets(segment, 4096, file);
		cJSON* object = cJSON_ParseWithLength(segment, 4096);
		cJSON* jNodes = cJSON_GetObjectItem(object, "nodes");
		if (cJSON_IsArray(jNodes) == true)
		{
			cJSON* jNode = NULL;
			cJSON_ArrayForEach(jNode, jNodes)
			{
				cJSON* jLat = cJSON_GetObjectItem(jNode, "lat");
				cJSON* jLon = cJSON_GetObjectItem(jNode, "lon");

				if (stockNodes[0].longitude == 0 && stockNodes[0].latitude == 0)
				{
					stockNodes[count].latitude = atof(cJSON_GetStringValue(jLat));
					stockNodes[count].longitude = atof(cJSON_GetStringValue(jLon));
					occurrences[count++]++;
				}
			
				else
				{
					int j = 0;
					bool found = false;
					while (stockNodes[j].latitude != 0 && stockNodes[j].longitude != 0)
					{
						if (atof(cJSON_GetStringValue(jLat)) == stockNodes[j].latitude
							&& atof(cJSON_GetStringValue(jLon)) == stockNodes[j].longitude)
						{
							occurrences[j]++;
							found = true;
						}

						j++;
					}

					if (!found)
					{
						stockNodes[count].latitude = atof(cJSON_GetStringValue(jLat));
						stockNodes[count].longitude = atof(cJSON_GetStringValue(jLon));
						occurrences[count++]++;
					}
				}
			}
		}
	}

	//(Affichage)
	for (int i = 0; i < 10000; i++)
	{
		if (occurrences[i] > 1)
		{
			printf("%d | %lf ; %lf\n", occurrences[i], stockNodes[i].latitude, stockNodes[i].longitude);
		}
	}

	//Création du graphe
	Graph* graph = Graph_create(count);
	for (int i = 0; i < count; i++)
	{
		Graph_setCoordinates(graph, i, stockNodes[i]);
	}

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
			cJSON_ArrayForEach(jNode, jNodes)
			{
				cJSON* jLat = cJSON_GetObjectItem(jNode, "lat");
				cJSON* jLon = cJSON_GetObjectItem(jNode, "lon");
				bool encountered = false;
				double distance = 0.0f;

				for (int i = 0; i < count; i++)
				{
					Point current = Graph_getCoordinates(graph, i);
					if (atof(cJSON_GetStringValue(jLat)) == current.latitude
						&& atof(cJSON_GetStringValue(jLon)) == current.longitude)
						encountered = true;

					if (encountered
						&& atof(cJSON_GetStringValue(jLat)) == current.latitude
						&& atof(cJSON_GetStringValue(jLon)) == current.longitude)
						distance = 0.0f;
				}
			}
		}
	}
}