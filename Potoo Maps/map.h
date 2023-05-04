#pragma once
#define INFINI 1000000

typedef struct Point_s
{
	/// @brief Lattitude du point
	double latitude;
	/// @brief Longitude du point
	double longitude;
}Point;


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