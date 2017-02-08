#include "Hough.h"
#include <math.h>
#include <cmath>
#include <memory.h>
#include "windowsdef.h"
#include "LogFile.h"
using namespace std;

CHough::CHough()
{
	_accu = NULL;
}

CHough::CHough(const AlgSensorData & lidar_data)
{
	_accu = NULL;
	Transform(lidar_data);
}


CHough::~CHough()
{
	if (_accu != NULL)
	{
		delete[] _accu;
		_accu = NULL;
	}
}

int CHough::Transform(const AlgSensorData & lidar_data)
{
	int numPoint = lidar_data.num;
	double max_x_y=0;
	double *x = new double[numPoint];
	double *y = new double[numPoint];
	for (int idx = 0;idx < numPoint; idx++)
	{
		x[idx] = lidar_data.localPosX[idx] / 100.0f;
		y[idx] = lidar_data.localPosY[idx] / 100.0f;
		if (abs((int)x[idx])>max_x_y)
		{
			max_x_y = abs((int)x[idx])+1;
		}
		if (abs((int)y[idx]) > max_x_y)
		{
			max_x_y = abs((int)y[idx])+1;
		}
	}
	 //Create the accu  
	double hough_h = max_x_y*2;//(sqrt(2.0) * (double)(max_x_y) / 2.0);
	_accu_h = hough_h * 2.0; // -r -> +r  
	_accu_w = 180;
	if (_accu!=NULL)
	{
		delete[] _accu;
		_accu = NULL;
	}
	_accu = new unsigned int[_accu_h * _accu_w];
	memset(_accu, 0, _accu_h * _accu_w*sizeof(unsigned int));
	for (int idx = 0;idx < numPoint; idx++)
	{
		for (int deg=0;deg<180; deg++)
		{
			double r = x[idx] * cos(deg*M_PI / 180.0) + y[idx]*sin(deg*M_PI / 180.0);
			_accu[(int)((round(r + hough_h) * 180.0)) + deg]++;
		}
	}
	delete[] x;
	delete[] y;

	return 0;
}

#ifdef NAVIPACK_WIN
std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> CHough::GetLines(int threshold)
{
	if (_accu==NULL)
	{
		return std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>();
	}
	std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > lines;

	int max_r = 0, max_deg = 0;
	int max_accu = 0;
	for (int r = 0;r < _accu_h; r++)
	{
		for (int deg = 0; deg < _accu_w; deg++)
		{
			if ((int)_accu[(r*_accu_w) + deg] >= threshold)
			{
				//Is this point a local maxima (9x9)  
				int max = _accu[(r*_accu_w) + deg];
				for (int ly = -4;ly <= 4;ly++)
				{
					for (int lx = -4;lx <= 4;lx++)
					{
						if ((ly + r >= 0 && ly + r < _accu_h) && (lx + deg >= 0 && lx + deg < _accu_w))
						{
							if ((int)_accu[((r + ly)*_accu_w) + (deg + lx)] > max)
							{
								max = _accu[((r + ly)*_accu_w) + (deg + lx)];
								ly = lx = 5;
							}
						}
					}
				}
				if (max > (int)_accu[(r*_accu_w) + deg])
					continue;

				if (max>max_accu)
				{
					max_accu = max;
					max_r = r;
					max_deg = deg;
				}

				int x1, y1, x2, y2;
				x1 = y1 = x2 = y2 = 0;
				if (deg >= 45 && deg <= 135)
				{
					//y = (r - x cos(t)) / sin(t)  
					x1 = 0;
					y1 = ((double)(r - (_accu_h / 2)) - x1 * cos(deg*M_PI / 180.0)) / sin(deg*M_PI / 180.0);
					x2 = 100;
					y2 = ((double)(r - (_accu_h / 2)) - x2 * cos(deg*M_PI / 180.0)) / sin(deg*M_PI / 180.0);
				}
				else
				{
					//x = (r - y sin(t)) / cos(t);  
					y1 = 0;
					x1 = ((double)(r - (_accu_h / 2)) - y1 * sin(deg*M_PI / 180.0)) / cos(deg*M_PI / 180.0);
					y2 = 100;
					x2 = ((double)(r - (_accu_h / 2)) - y2 * sin(deg*M_PI / 180.0)) / cos(deg*M_PI / 180.0);
				}
				lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int> >(std::pair<int, int>(x1, y1), std::pair<int, int>(x2, y2)));
			}
		}
	}

	if (max_accu>0)
	{
		int r = max_r;
		int deg = max_deg;

		int x1, y1, x2, y2;
		x1 = y1 = x2 = y2 = 0;
		if (deg >= 45 && deg <= 135)
		{
			//y = (r - x cos(t)) / sin(t)  
			x1 = 0;
			y1 = ((double)(r - (_accu_h / 2)) - x1 * cos(deg*M_PI / 180.0)) / sin(deg*M_PI / 180.0);
			x2 = 100;
			y2 = ((double)(r - (_accu_h / 2)) - x2 * cos(deg*M_PI / 180.0)) / sin(deg*M_PI / 180.0);
		}
		else
		{
			//x = (r - y sin(t)) / cos(t);  
			y1 = 0;
			x1 = ((double)(r - (_accu_h / 2)) - y1 * sin(deg*M_PI / 180.0)) / cos(deg*M_PI / 180.0);
			y2 = 100;
			x2 = ((double)(r - (_accu_h / 2)) - y2 * sin(deg*M_PI / 180.0)) / cos(deg*M_PI / 180.0);
		}
		lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int> >(std::pair<int, int>(x1, y1), std::pair<int, int>(x2, y2)));
	}
	LOGD("[CHough::GetLines] lines: %d  based on threshold  %d\n ", lines.size()-1, threshold);

	return lines;

}

#endif
