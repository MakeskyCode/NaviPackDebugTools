#pragma once
#include "SDKProtocol.h"
#include <vector>
using namespace std;
class CHough
{
public:
	CHough();

	CHough(const AlgSensorData& lidar_data);

	~CHough();

	int Transform(const AlgSensorData& lidar_data);

	vector< std::pair< std::pair<int, int>, std::pair<int, int> > > GetLines(int threshold);

private:
	int _accu_h;
	int _accu_w;
	unsigned int *_accu;
};

