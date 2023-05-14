#pragma once

#include "settings.h"
#include "cJSON.h"
#include "path.h"
#include "graph.h"
#include "ShortestPath.h"
#include "dict.h"

/// @brief Vérifie que deux points ont les mêmes coordonnées
/// @param a le premier point
/// @param b le deuxième point
bool sameCoordinates(Point a, Point b);

/// @brief
/// param in a, 1er point
/// param in b, 2e point
/// return la distance en metre entre a et b
double Distance(Point a, Point b);

/// @brief
/// param in start, point de depart
/// param in stockNodes, tous les points de la region
/// param in count, nombre de points de la region
/// return le point le plus proche de start
int Get_NearestPoint(Point start, Point* stockNodes, int count);

/// @brief Récupère des coordonnées entrées par l'utilisateur pour les transformer en points
/// @param coordinates le tableau de coordonnées pour chaque intersection
/// @param count le nombre total d'intersections
/// @param start le point de départ du trajet
/// @param end le point d'arrivée du trajet
void inputCoordinates(Point* coordinates, int count, Point* start, Point* end, int* idStart, int* idEnd);

/// @brief Parse le fichier pour en tirer uniquement les intersections.
/// Renvoie le nombre total d'intersections trouvï¿½es
/// @param file le fichier ï¿½ parser
/// @param coordinates le tableau de coordonnï¿½es pour chaque intersection
Point* parsingFile(FILE* file, int* count);

/// @brief Parse le fichier une deuxiï¿½me fois pour constituer un graphe
/// @param graph le graphe
/// @param count le nombre total d'intersections
/// @param coordinates le tableau de coordonnï¿½es pour chaque intersection
/// @param file le fichier ï¿½ parser
void graphMap(Graph* graph, int count, Point* coordinates, FILE* file);

/// @brief Cherche le chemin le plus court d'un point A ï¿½ un point B.
/// Renvoie la taille du chemin
/// @param graph le graphe
/// @param idA le point de dï¿½part
/// @param idB le point d'arrivï¿½e
/// @param route le chemin trouvï¿½
int findPath(Graph* graph, int idA, int idB, Point* route);

/// @brief Ecrit le fichier de sortie
/// @param route le chemin trouvï¿½
/// @param size la taille du chemin
void writeOutput(Point* route, int size);

/// @brief Ecrit le fichier de pre-traitement
/// @param graph le graph cree
/// @param count le nombre de noeud
void writeTraitement(Graph* graph, int count);

Graph* readTraitement();