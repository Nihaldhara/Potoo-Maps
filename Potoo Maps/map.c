#include "map.h"

#define CHARGEMENT 80

double Distance(Point a, Point b)
{
	//Calculons les composantes en radian
	double alat = a.latitude * DEG_TO_RAD;
	double blat = b.latitude * DEG_TO_RAD;
	double alon = a.longitude * DEG_TO_RAD;
	double blon = b.longitude * DEG_TO_RAD;

	//On cree les sinus et cosinus des latitudes
	double sina = sin(alat);
	double sinb = sin(blat);
	double cosa = cos(alat);
	double cosb = cos(blat);
	//Le cosinus des longitudes
	double cosab = cos(blon - alon);
	
	double res = ((sina * sinb) + (cosa * cosb * cosab));
	return (6378137 * acos(res));
}


Point Get_NearestPoint(Point start, Point* stockNodes, int count)
{
	double distance = INTMAX_MAX;
	int res = 0;
	for (int i = 0; i < count; i++)
	{
	
		if (distance >= Distance(start, stockNodes[i]))
		{
			distance = Distance(start, stockNodes[i]);
			res = i;
		}
	}
	return stockNodes[res];
}

void inputCoordinates(Point* coordinates, int count, Point* start, Point* end)
{
	printf("\n\n\nEntrez ici les coordonnees de votre point de depart : \n\n");
	printf("Longitude : ");
	scanf("%lf", &start->latitude);
	printf("Latitude : ");
	scanf("%lf", &start->longitude);
	printf("\n\nPuis les coordonnees de votre point d'arrivee : \n\n");
	printf("Longitude : ");
	scanf("%lf", &end->latitude);
	printf("Latitude : ");
	scanf("%lf", &end->longitude);
	printf("\n\n\n");

	*start = Get_NearestPoint(*start, coordinates, count);
	*end = Get_NearestPoint(*end, coordinates, count);
}

int parsingFile(FILE* file, Point* coordinates)
{
	//Calcul de la longueur maximale du fichier et initialisation de la barre de chargement
	long total, atm, previous = 0;
	fseek(file, 0, SEEK_END);
	total = ftell(file);
	rewind(file);
	(float)total /= CHARGEMENT;
	printf("Lecture du fichier en cours\n"); // La barre de chargement doit etre égal a n-1, n était le diviseur de "total"
	for (int i = 0; i < CHARGEMENT - 1; i++)
		printf("-");
	printf("\n");

	//Boucle de lecture et stockage des différentes intersections
	int count = 0;
	char* segment = calloc(4096, sizeof(char));
	int* tempOccurrences = (int*)calloc(10000, sizeof(int));
	Point* tempCoordinates = (Point*)calloc(10000, sizeof(Point));
	while (!feof(file))
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
				for (int j = 0; j < count; j++)
				{
					if (dLat == tempCoordinates[j].latitude
						&& dLon == tempCoordinates[j].longitude)
					{
						tempOccurrences[j]++;
						found = true;
						break;
					}
				}

				if (!found)
				{
					tempCoordinates[count].latitude = dLat;
					tempCoordinates[count].longitude = dLon;
					tempOccurrences[count++]++;
				}
			}
		}
	}

	//Rectifications parce que je sais pas coder proprement
	int finalCount = 0;
	int* occurrences = (int*)calloc(count, sizeof(int));
	printf("\n");
	for (int i = 0; i < count; i++)
	{
		if (tempOccurrences[i] > 1)
		{
			occurrences[finalCount] = tempOccurrences[i];
			coordinates[finalCount++] = tempCoordinates[i];
		}
	}

	return finalCount;
}

void graphMap(Graph* graph, int count, Point* coordinates, FILE* file)
{
	char* segment = calloc(4096, sizeof(char));
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
}

int findPath(Graph* graph, int idA, int idB, Point* route)
{
	Path* path = Graph_shortestPath(graph, 148, 14);
	IntListNode* current = path->list->sentinel.next;
	int size = 0;
	while (current != &path->list->sentinel)
	{
		route[size++] = Graph_getCoordinates(graph, current->value);
		current = current->next;
	}

	return size;
}

void writeOutput(Point* route, int size)
{
	FILE* output = fopen("../output.json", "w");
	char* bFile = "{\n		\"type\": \"FeatureCollection\",\n		\"features\" : [\n	{\n		\"type\": \"Feature\",\n			\"geometry\" :\n		{\n			\"type\": \"LineString\",\n				\"coordinates\" : [\n";
	char* eFile = "				]\n			}\n		}\n	]\n}\n";

	fprintf(output, bFile);
	for (int i = 0; i < size; i++)
	{
		if (i + 1 == size)
			fprintf(output, "					[%lf, %lf]\n", route[i].longitude, route[i].latitude);
		else
			fprintf(output, "					[%lf, %lf],\n", route[i].longitude, route[i].latitude);
	}
	fprintf(output, eFile);
}