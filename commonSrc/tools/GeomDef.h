
#ifndef __GEOM_DEF_H__
#define __GEOM_DEF_H__

#include <math.h>
#include <algorithm>
#include <vector>
#include "tools/windowsdef.h"
#include "SDKProtocol.h"

using namespace std;
struct TVector
{
	double len;
	double phi;

	TVector()
	{
		len = 0;
		phi = 0;
	}

	TVector(double _len, double _phi)
	{
		len = _len;
		phi = _phi;
	}
};


//pose struct 
struct TPose
{
	float x;
	float y;
	float phi;
	TPose() {}
	TPose(float _x, float _y, float _phi) { x = _x, y = _y, phi = _phi; }
    float GetDistance(TPose point)
    {
		return sqrt((x - point.x)*(x - point.x) + (y - point.y)*(y - point.y));
    }
};

TPose composeFrom(const TPose& a,const TPose& b);

//point struct
struct TPoint
{
	float x;
	float y;
	float norm(){ return sqrt(x*x+y*y); }
	TPoint() {}
	TPoint(float _x, float _y) { x = _x, y = _y; }
	TPoint(TPose pose) { x = pose.x; y = pose.y; }
	float GetDistance(TPoint pt)
	{
		return  (float)sqrt((pt.x - x)*(pt.x - x) + (pt.y - y)*(pt.y - y));
	}
	TPoint operator+(const TPoint & a)
	{
		TPoint t1;
		t1.x = x + a.x;
		t1.y = y + a.y;
		return t1;
	}
};

struct TPixcelPoint
{
	int x;
	int y;
	TPixcelPoint() : x(0), y(0) {}
	TPixcelPoint(int _x, int _y) : x(_x), y(_y) {}

	TPixcelPoint operator+(const TPixcelPoint & a)
	{
		TPixcelPoint t1;
		t1.x = x + a.x;
		t1.y = y + a.y;
		return t1;
	}

	float GetDistance(TPixcelPoint pt)
	{
		return  (float)sqrt((float)(pt.x - x)*(pt.x - x) + (float)(pt.y - y)*(pt.y - y));
	}
};

//point struct
struct TPolarPoint
{
	float dis;
	float phi;
	TPolarPoint();
	TPolarPoint(float _dis, float _phi);
};

//speed struct
struct TSpeed
{
	float v;
	float w;
	TSpeed(float _x = 0, float _y = 0){ v = _x, w = _y; };
};


/*---------------------------------------------------------------
communication data struct
---------------------------------------------------------------*/
struct  TTargetPoint
{
	TPoint  pt;
	float   phi;			//目标停止角度
	TTargetPoint()
	{
		pt = TPoint(0, 0);
		phi = -1;
	}
	TTargetPoint(TPoint _pt, float _phi = -1)
	{
		pt = _pt;
		phi = _phi;
	}

	double GetDistance(TPoint &point)
	{
		return pt.GetDistance(point);
	}
	double GetDistance(TPose &pose)
	{
		return pt.GetDistance(pose);
	}
};

struct  TParolPoints
{
	int points_num;
	TTargetPoint point[20];
};


struct TPolarPointS32
{
	s32 dis;
	s32 phi;

	TPolarPointS32(s32 _dis = 0, s32 _phi = 0){ dis = _dis; phi = _phi; };
};

struct TPath
{
	int				num;
	TPoint			path_point[MAX_TARGET_LIST_SIZE];		//A*规划出来的全局路径
	double			target_phi;
};

bool IsEqualTwoPath(std::vector<TPose> a, std::vector<TPose> b);

double GetCornerAngle(TPose p1, TPose p2, TPose p3);
void CalculateFittingLine(std::vector< TPoint>points, float &a0, float &a1);
#endif