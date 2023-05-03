#include "settings.h"
#include "cJSON.h"
#include "graph.h"
#include "parsing.h"
#include "map.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	//Là je choppe le tout dernier point, parce que pourquoi pas
	/*while (!feof(file))
		fgets(test, 4096, file);
	//Ici je transforme la lecture de mon fichier en objet, puis en un truc que je peux print
	cJSON* object = cJSON_ParseWithLength(test, 4096);
	char* print = cJSON_Print(object);
	//Et là je découpe uniquement les nodes du segment
	cJSON* jNodes = cJSON_GetObjectItem(object, "nodes");
	print = cJSON_Print(jNodes);

	//Là je print les lattitudes et longitudes proprement
	if (cJSON_IsArray(jNodes) == true)
	{
		cJSON* jNode = NULL;
		cJSON_ArrayForEach(jNode, jNodes)
		{
			cJSON* jLat = cJSON_GetObjectItem(jNode, "lat");
			cJSON* jLon = cJSON_GetObjectItem(jNode, "lon");

			printf("lon : %lf  |  lat : %lf\n", 
				atof(cJSON_GetStringValue(jLon)), atof(cJSON_GetStringValue(jLat)));
		}
	}*/

	

	//Création du graphe
	//   -Tri des différents points :
	int count = 0;
	int* occurrences = (int*)calloc(10000, sizeof(int));
	Point* stockNodes = (Point*)calloc(10000, sizeof(Point));
	FILE* file = fopen("../Potoo Maps/esiea.geojson", "r");
	char* test = calloc(4096, sizeof(char));
	for (int i = 0; i < 156; i++)
		fgets(test, 4096, file);

	//Boucle de lecture et stockage des différentes intersections
	while(!feof(file))
	{
		fgets(test, 4096, file);
		cJSON* object = cJSON_ParseWithLength(test, 4096);
		cJSON* jNodes = cJSON_GetObjectItem(object, "nodes");
		if (cJSON_IsArray(jNodes) == true)
		{
			cJSON* jNode = NULL;
			cJSON_ArrayForEach(jNode, jNodes)
			{
				cJSON* jLat = cJSON_GetObjectItem(jNode, "lat");
				cJSON* jLon = cJSON_GetObjectItem(jNode, "lon");

				if (stockNodes[0].longitude == 0 && stockNodes[0].lattitude == 0)
				{
					stockNodes[count].lattitude = atof(cJSON_GetStringValue(jLat));
					stockNodes[count].longitude = atof(cJSON_GetStringValue(jLon));
					occurrences[count++]++;
				}

				else
				{
					int j = 0;
					bool found = false;
					while (stockNodes[j].lattitude != 0 && stockNodes[j].longitude != 0)
					{
						if (atof(cJSON_GetStringValue(jLat)) == stockNodes[j].lattitude
							&& atof(cJSON_GetStringValue(jLon)) == stockNodes[j].longitude)
						{
							occurrences[j]++;
							found = true;
						}

						j++;
					}

					if (!found)
					{
						stockNodes[count].lattitude = atof(cJSON_GetStringValue(jLat));
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
			printf("%d | %lf ; %lf\n", occurrences[i], stockNodes[i].lattitude, stockNodes[i].longitude);
		}
	}

	//Création du graphe
	
}