#pragma once

typedef struct Point_s
{
	/// @brief Lattitude du point
	double latitude;
	/// @brief Longitude du point
	double longitude;
}Point;


double Distance(Point a, Point b);