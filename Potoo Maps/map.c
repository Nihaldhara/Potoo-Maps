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