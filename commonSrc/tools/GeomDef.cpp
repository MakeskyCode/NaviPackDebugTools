#include "GeomDef.h"
#include "CommonFunctions.h"


TPose composeFrom(const TPose& a, const TPose& b)
{
	TPose p;
	// Use temporary variables for the cases (A==this) or (B==this)
	const double new_x = a.x + b.x * cos(a.phi) - b.y * sin(a.phi);
	const double new_y = a.y + b.x * sin(a.phi) - b.y * cos(a.phi);
	p.x = new_x;
	p.y = new_y;

	p.phi = NormalToPI(a.phi + b.phi);

	return p;
}

bool IsEqualTwoPath(std::vector<TPose> a, std::vector<TPose> b)
{
	if (a.size() == b.size())
	{
		int size = a.size();
		for (int i = 0; i < size; i++)
		{
			if ((a[i].x - b[i].x)*(a[i].x - b[i].x) + (a[i].y - b[i].y)*(a[i].y - b[i].y)>0.01)
				return false;
		}
		return true;
	}
	return false;
}

double GetCornerAngle(TPose p1, TPose p2, TPose p3)
{
	double dis_p12 = pow((p1.x - p2.x), (float)2) + pow((p1.y - p2.y), (float)2);
	double dis_p32 = pow((p3.x - p2.x), (float)2) + pow((p3.y - p2.y), (float)2);
	double dis_p13 = pow((p1.x - p3.x), (float)2) + pow((p1.y - p3.y), (float)2);

	double cos_angle = (dis_p12 + dis_p32 - dis_p13) / (2 * sqrt(dis_p12)*sqrt(dis_p32));

	return (M_PI - acos(cos_angle));
}

void CalculateFittingLine(std::vector< TPoint>points, float &a0, float &a1)
{

}