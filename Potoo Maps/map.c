#include "map.h"

#define CHARGEMENT 100
#define TERRE 6378137

bool sameCoordinates(Point a, Point b)
{
	if (a.latitude == b.latitude
		&& a.longitude == b.longitude)
		return true;
	else return false;
}

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
	return (TERRE * acos(res));
}


int Get_NearestPoint(Point start, Point* stockNodes, int count)
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

	return res;
}

void inputCoordinates(Point* coordinates, int count, Point* start, Point* end, int* idStart, int* idEnd)
{
	printf("\n\n\nEntrez ici les coordonnees de votre point de depart : \n\n");
	printf("Latitude : ");
	scanf("%lf", &start->latitude);
	printf("Longitude : ");
	scanf("%lf", &start->longitude);
	printf("\n\nPuis les coordonnees de votre point d'arrivee : \n\n");
	printf("Latitude : ");
	scanf("%lf", &end->latitude);
	printf("Longitude : ");
	scanf("%lf", &end->longitude);
	printf("\n\n\n");

	/*
	start->latitude = 48.080862238743386;
	start->longitude = -0.7481967545409075;
	end->latitude = 48.09371262498784;
	end->longitude = -0.7484873470449195;
	*/
	*idStart = Get_NearestPoint(*start, coordinates, count);
	*idEnd = Get_NearestPoint(*end, coordinates, count);
}

Point* parsingFile(FILE* file, int* intersections)
{
	//Calcul de la longueur maximale du fichier et initialisation de la barre de chargement
	long total, atm, previous = 0;
	fseek(file, 0, SEEK_END);
	total = ftell(file);
	rewind(file);
	float tmp = (float)total / (float)CHARGEMENT;
	printf("Lecture du fichier en cours\n");
	for (int i = 0; i < CHARGEMENT; i++)
		printf("-");
	printf("\n");

	//Boucle de lecture et stockage des differents points
	int count = 0, avancement = 0, tmp2;
	char* segment = calloc(4096, sizeof(char));
	/*int* tempOccurrences = (int*)calloc(10000, sizeof(int));
	Point* tempCoordinates = (Point*)calloc(10000, sizeof(Point));*/
	Dict* dict = Dict_New();
	while (fgets(segment, 4096, file))
	{
		// tester si Item c'est vraiment un string

		//Calcul du pourcentage d'avancement de la barre de chargement
		atm = ftell(file);
		if ((float)atm >= ((float)previous + tmp))
		{
			avancement++;
			previous = avancement * tmp;
			printf("#");
		}

		tmp2 = strnlen(segment, 4096);
		cJSON* object = cJSON_ParseWithLength(segment, 4096);
		cJSON* jType = cJSON_GetObjectItem(object, "type");

		if (!cJSON_IsString(jType))
		{
			cJSON_Delete(object);
			continue;
		}

		if (strcmp(cJSON_GetStringValue(jType), "way") != 0)
		{
			cJSON_Delete(object);
			continue;
		}

		cJSON* jNodes = cJSON_GetObjectItem(object, "nodes");
		if (cJSON_IsArray(jNodes) == true)
		{
			cJSON* jNode = NULL;
			cJSON_ArrayForEach(jNode, jNodes)
			{
				cJSON* jLat = cJSON_GetObjectItem(jNode, "lat");
				cJSON* jLon = cJSON_GetObjectItem(jNode, "lon");

				if (!cJSON_IsString(jLat))
				{
					cJSON_Delete(object);
					continue;
				}

				if (!cJSON_IsString(jLon))
				{
					cJSON_Delete(object);
					continue;
				}
				double dLat = atof(cJSON_GetStringValue(jLat));
				double dLon = atof(cJSON_GetStringValue(jLon));
				Point coordinates;
				coordinates.latitude = dLat;
				coordinates.longitude = dLon;

				Dict_Insert(dict, coordinates, 1);
				if (Dict_Get(dict, coordinates) == 2) count++;
			}
		}
		cJSON_Delete(object);
	}

	//Recherche des intersections
	int finalCount = 0;
	Point* coordinates = (Point*)calloc(count, sizeof(coordinates));
	DictIter* iter = (DictIter*)calloc(1, sizeof(DictIter));
	KVPair* pair = (KVPair*)calloc(1, sizeof(KVPair));
	Dict_GetIterator(dict, iter);
	printf("\n");
	for (int i = 0; i < count; i++)
	{
		pair = DictIter_Next(iter);
		if (pair->value > 1)
		{
			coordinates[finalCount++] = pair->key;
		}
	}
	*intersections = finalCount;

	free(segment);
	return coordinates;
}

void graphMap(Graph* graph, int count, Point* coordinates, FILE* file)
{
	char* segment = calloc(4096, sizeof(char));
	rewind(file);
	while (fgets(segment, 4096, file))
	{
		cJSON* object = cJSON_ParseWithLength(segment, 4096);
		cJSON* jType = cJSON_GetObjectItem(object, "type");

		if (!cJSON_IsString(jType))
		{
			cJSON_Delete(object);
			continue;
		}

		if (strcmp(cJSON_GetStringValue(jType), "way") != 0)
		{
			cJSON_Delete(object);
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

				if (!cJSON_IsString(jLat))
				{
					cJSON_Delete(object);
					continue;
				}

				if (!cJSON_IsString(jLon))
				{
					cJSON_Delete(object);
					continue;
				}

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
		cJSON_Delete(object);
	}

	free(segment);
}

int findPath(Graph* graph, int idA, int idB, Point* route)
{
	Path* path = Graph_shortestPath(graph, idA, idB);
	if (!path)
	{		
		printf("il n'y a pas de Path");
		Path_destroy(path);
		return 0;
	}
	IntListNode* current = path->list->sentinel.next;
	int size = 0;
	while (current != &path->list->sentinel)
	{
		route[size++] = Graph_getCoordinates(graph, current->value);
		current = current->next;
	}
	Path_destroy(path);
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
	fclose(output);
}



void writeTraitement(Graph* graph, int count)
{
	FILE* pretraitement = fopen("../Potoo Maps/pretraitement.txt","wb");
	fprintf(pretraitement, "%d\n", count);

	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			float poids = Graph_get(graph, i, j);
			if (poids >= 0)
			{
				fprintf(pretraitement,"%f %d %d\n", poids, i , j);
			}
		}
	}
	fclose(pretraitement);
}

Graph* readTraitement()
{
	FILE* pretraitement = fopen("../Potoo Maps/pretraitement.txt","rb");
	if (!pretraitement)
		return NULL;
	int count, u, v;
	long scan;
	float w;
	scan = fscanf(pretraitement, "%d", &count);

	Graph* graph = Graph_create(count);
	while(fscanf(pretraitement, "%f %d %d", &w, &u, &v))
	{
		printf("%d, %d, %f\n", u, v, w);
		Graph_set(graph, u, v, w);
	}
	fclose(pretraitement);
	return graph;
}