#include "settings.h"
#include "cJSON.h"
#include "graph.h"
#include "parsing.h"
#include "map.h"
#include <stdlib.h>
#include <stdio.h>

#define CHARGEMENT 80

int main()
{
	//Cr�ation du graphe
	//   -Tri des diff�rents points :
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
	printf("Lecture du fichier en cours\n"); // La barre de chargement doit etre �gal a n-1, n �tait le diviseur de "total"
	for (int i = 0; i < CHARGEMENT-1; i++)
		printf("-");
	printf("\n");

for (int i = 0; i < 156; i++)
		fgets(segment, 4096, file);

	//Boucle de lecture et stockage des diff�rentes intersections
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
	for (int i = 0; i < 10000; i++)
	{
		if (occurrences[i] > 1)
		{
			printf("%d | %lf %lf\n", occurrences[i], stockNodes[i].latitude, stockNodes[i].longitude);
		}
	}

	//Cr�ation du graphe
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