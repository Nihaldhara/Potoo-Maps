#pragma once

#include "settings.h"
#include "cJSON.h"
#include "path.h"
#include "graph.h"

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
Point Get_NearestPoint(Point start, Point* stockNodes, int count);

void inputCoordinates(Point* coordinates, int count, Point* start, Point* end);

/// @brief Parse le fichier pour en tirer uniquement les intersections.
/// Renvoie le nombre total d'intersections trouvées
/// @param file le fichier à parser
/// @param coordinates le tableau de coordonnées pour chaque intersection
int parsingFile(FILE* file, Point* coordinates);

/// @brief Parse le fichier une deuxième fois pour constituer un graphe
/// @param graph le graphe
/// @param count le nombre total d'intersections
/// @param coordinates le tableau de coordonnées pour chaque intersection
/// @param file le fichier à parser
void graphMap(Graph* graph, int count, Point* coordinates, FILE* file);

/// @brief Cherche le chemin le plus court d'un point A à un point B.
/// Renvoie la taille du chemin
/// @param graph le graphe
/// @param idA le point de départ
/// @param idB le point d'arrivée
/// @param route le chemin trouvé
int findPath(Graph* graph, int idA, int idB, Point* route);

/// @brief Ecrit le fichier de sortie
/// @param route le chemin trouvé
/// @param size la taille du chemin
void writeOutput(Point* route, int size);