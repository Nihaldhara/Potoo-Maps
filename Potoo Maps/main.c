#include "settings.h"
#include "cJSON.h"
#include "graph.h"
#include "parsing.h"
#include "map.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	//Cr�ation du graphe
	//   -Tri des diff�rents points :
	int count = 0;
	int* occurrences = (int*)calloc(10000, sizeof(int));
	Point* stockNodes = (Point*)calloc(10000, sizeof(Point));
	FILE* file = fopen("../Potoo Maps/esiea.geojson", "r");
	char* test = calloc(4096, sizeof(char));
	for (int i = 0; i < 156; i++)
		fgets(test, 4096, file);

	//Calcul de la longueur maximale du fichier et initialisation de la barre de chargement
	long total, atm, previous = 0;
	fseek(file, 0, SEEK_END);
	total = ftell(file);
	rewind(file);
	(float)total /= 20.0f;
	printf("Lecture du fichier\n-------------------\n"); // La barre de chargement doit etre �gal a n-1, n �tait le diviseur de "total"

	//Boucle de lecture et stockage des diff�rentes intersections
	while(!feof(file))
	{
		fgets(test, 4096, file);
		cJSON* object = cJSON_ParseWithLength(test, 4096);
		cJSON* jNodes = cJSON_GetObjectItem(object, "nodes");
		
		//Calcul du pourcentage d'avancement de la barre de cahrgement
		atm = ftell(file);
		float res = (float)atm / (float)total;
		if ((float)atm >= ((float)previous + (float)total))
		{
			previous = atm;
			printf("#");
		}
		

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
	printf("\n");
	for (int i = 0; i < 10000; i++)
	{
		if (occurrences[i] > 1)
		{
			printf("%d | %lf ; %lf\n", occurrences[i], stockNodes[i].latitude, stockNodes[i].longitude);
		}
	}

	//Cr�ation du graphe
	
}