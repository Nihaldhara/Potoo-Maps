#pragma once

#include "settings.h"
#include "cJSON.h"
#include "path.h"
#include "graph.h"
#include "ShortestPath.h"
#include "dict.h"

/// @brief V�rifie que deux points ont les m�mes coordonn�es
/// @param a le premier point
/// @param b le deuxi�me point
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

/// @brief R�cup�re des coordonn�es entr�es par l'utilisateur pour les transformer en points
/// @param coordinates le tableau de coordonn�es pour chaque intersection
/// @param count le nombre total d'intersections
/// @param start le point de d�part du trajet
/// @param end le point d'arriv�e du trajet
void inputCoordinates(Point* coordinates, int count, Point* start, Point* end, int* idStart, int* idEnd);

/// @brief Parse le fichier pour en tirer uniquement les intersections.
/// Renvoie le nombre total d'intersections trouv�es
/// @param file le fichier � parser
/// @param coordinates le tableau de coordonn�es pour chaque intersection
Point* parsingFile(FILE* file, int* count);

/// @brief Parse le fichier une deuxi�me fois pour constituer un graphe
/// @param graph le graphe
/// @param count le nombre total d'intersections
/// @param coordinates le tableau de coordonn�es pour chaque intersection
/// @param file le fichier � parser
void graphMap(Graph* graph, int count, Point* coordinates, FILE* file);

/// @brief Cherche le chemin le plus court d'un point A � un point B.
/// Renvoie la taille du chemin
/// @param graph le graphe
/// @param idA le point de d�part
/// @param idB le point d'arriv�e
/// @param route le chemin trouv�
int findPath(Graph* graph, int idA, int idB, Point* route);

/// @brief Ecrit le fichier de sortie
/// @param route le chemin trouv�
/// @param size la taille du chemin
void writeOutput(Point* route, int size);

/// @brief Ecrit le fichier de pre-traitement
/// @param graph le graph cree
/// @param count le nombre de noeud
void writeTraitement(Graph* graph, int count);

Graph* readTraitement();