#include "settings.h"
#include "map.h"

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
	double distance = INFINI;
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