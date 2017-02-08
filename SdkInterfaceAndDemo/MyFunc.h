#ifndef _MY_FUNC
#define _MY_FUNC

#include <vector>
#include "fstream"
#include <iostream>
#include <iostream>
#include <string>

#include "tools/GeomDef.h"

//#include "slam/PoseProviderData.h"

#define M_PI 3.1415926

using namespace std;

/** \brief 实际坐标与像素之间的转换 */
extern int stretch_scale_buf;
extern int stretch_scale;

/** \brief 辅助函数 */
void MarshalString(System::String ^ s, std::string& os);

/** \brief 颜色信息定义 */
#define		RED			CV_RGB(255, 0, 0)
#define		GREEN		CV_RGB(0, 255, 0)
#define		BLUE		CV_RGB(0, 0, 255)

#define		YELLOW		CV_RGB(255, 255, 0)
#define		ORANGE		CV_RGB(255, 126, 0)
#define		SPRINGGREEN	CV_RGB(0, 255, 255)



struct TIcpSlamData
{
	TPose fusingPose;
	TPose slamPose;
	float lidarData[360];
};

//struct TMapParam
//{
//public:
//	TMapParam()
//	{
//		width = height = 0;
//		resolution = x_min = y_min = 0;
//	}
//public:
//	int   width;
//	int   height;
//	float resolution;
//	float x_min;
//	float y_min;
//};


/** \brief 记录上位机的配置信息 */
class SysParamIniFile
{
public:
	struct SysParam
	{
		char	ip[30];
		int		port;
		int		scale;
		char	map_name[30];
		char	mode[30];
	};

	SysParam sys_param;
	std::ifstream m_fin;
	std::ofstream m_fout;
	std::string file_name_;

	SysParamIniFile()
	{
		file_name_ = "..//data//sys_param.ini";
	}

	bool WriteParam()
	{
		const char * file = file_name_.c_str();
		m_fout.open(file, ios_base::out | ios_base::binary);
		// New file
		if (m_fout.is_open())
		{
			m_fout.write((char *)&sys_param, sizeof(SysParam));
			m_fout.close();
			return true;
		}
		else
		{
			cout << "Can not open the output file \n"  << endl;
			exit(EXIT_FAILURE);
		}
		return true;
	}

	bool ReadParam()
	{
		m_fin.open(file_name_.c_str(), ios_base::in | ios_base::binary);
		if (m_fin.is_open())
		{
			m_fin.read((char *)&sys_param, sizeof(SysParam));
			printf("param   ip:%s   port:%d-------------\n", sys_param.ip, sys_param.port);
			m_fin.close();
			return true;
		}
		else
		{
			cout << "Can not open the output file \n" << endl;
			exit(EXIT_FAILURE);
			return false;
		}
	}
};
/** \brief 地图的文件信息 */
class PicHeadIni
{
public:
	//TMapParam map_param_;

	std::ifstream m_fin;
	std::ofstream m_fout;
	std::string file_name_;

	PicHeadIni(std::string file_name)
	{
		file_name_ = file_name;
	}

	bool WriteParam()
	{
		//file_name += ".ini";
		file_name_ = "..//data//" + file_name_;
		const char * file = file_name_.c_str();
		m_fout.open(file, ios_base::out | ios_base::binary);
		// New file
		if (m_fout.is_open())
		{
			//m_fout.write((char *)&map_param_, sizeof(map_param_));
			m_fout.close();
			return true;
		}
		else
		{
			cout << "Can not open the output file \n" << endl;
			exit(EXIT_FAILURE);
		}
		return true;
	}

	bool ReadParam()
	{
		file_name_ = "..//data//" + file_name_;
		m_fin.open(file_name_.c_str(), ios_base::in | ios_base::binary);
		// New file??
		if (m_fin.is_open())
		{
			/*m_fin.read((char *)&map_param_, sizeof(map_param_));
			printf("pic head param  %d %d %f %f %f\n", map_param_.width, map_param_.height, map_param_.resolution, map_param_.x_min, map_param_.y_min);*/
			m_fin.close();
			return true;
		}
		else
		{
			cout << "Can not open the output file \n" << endl;
			exit(EXIT_FAILURE);
			return false;
		}
	}
};

#endif