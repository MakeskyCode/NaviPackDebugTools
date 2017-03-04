#ifdef NAVIPACK_WIN
#include "MyForm.h"

#include <iostream>
#include "opencv2/opencv.hpp"

#include "NaviPackSDK.h"
#include "MyFunc.h"
#include"tools\LogFile.h"
#include "hardware_interface/TCPClientInterface.h"
#include "hardware_interface/SerialPortInterface.h"
#include <windows.h>
#include "visualScrop/VisualScope.h"
#include "tools/CriticalSection.h"

using namespace std;
using namespace System;
using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace ProjectInterface;

#ifndef M_PIf
#define M_PIf  3.14159265358979f
#endif

//用户使用变量
int navipackInterfaceId = -1;
AlgMapData *algMapData = new AlgMapData;
AlgSensorData *algLidarMapData = new AlgSensorData;
AlgSensorData *algRealLidarData = new AlgSensorData;


//绘图使用
TMapParam param;
bool paramReady = false;
float pixel_size = 0.1;
int pp_robot_radius = 0;
vector<TPoint> motion_trajectory;//运动轨迹
CriticalSection motion_trajectory_cs;
CvVideoWriter* video = NULL;
TCPClientInterface g_TCPClientInterface(NETWORK_INTERFACE_ID,2*1024*1024);
SerialPortInterface g_SerPortInterface(MCU_INTERFACE_ID,128*1024);
int gTcpId;

//速度控制
float v = 0;
float w = 0;

//虚拟墙
AlgMapData *virtual_wall_map = new AlgMapData;
vector<TPoint> virtual_obstacles;

#pragma pack(push, 1) 
typedef struct _Mpu6500Data
{
	float acce_z;
	float gyro_z;
	float acture_z;
	float angular;
}Mpu6500Data;
#pragma pack(pop)

void clearMotionTrajectory()
{
	motion_trajectory_cs.Enter();
	motion_trajectory.clear();
	motion_trajectory_cs.Leave();
}

int getMotionTrajectoryPixcel(vector<TPixcelPoint>& pixel_trajectory)
{
	motion_trajectory_cs.Enter();
	int size = motion_trajectory.size();
	for (int i=0; i<size; i++)
	{
		TPixcelPoint pt;
		pt.x = (int)((motion_trajectory[i].x - param.x_min) / pixel_size);
		pt.y = param.height*stretch_scale - (int)((motion_trajectory[i].y - param.y_min) / pixel_size);
		pixel_trajectory.push_back(pt);
	}
	motion_trajectory_cs.Leave();
	return size;
}

void addMotionTrajectoryPoint(TPoint cur_pos)
{
	motion_trajectory_cs.Enter();
	motion_trajectory.push_back(cur_pos);
	motion_trajectory_cs.Leave();
}

/** \brief 实际坐标转为像素坐标 */
void TPoint2Pixel_Scale(TPoint &p1, TPixcelPoint &p2)
{
	p2.x = (int)((p1.x - param.x_min) / pixel_size);
	p2.y = param.height*stretch_scale - (int)((p1.y - param.y_min) / pixel_size);
}

/** \brief 像素坐标转为世界座标 */
void Pixel2TPoint_Scale(TPixcelPoint &p1, TPoint &p2)
{
	p2.x = p1.x*pixel_size + param.x_min;
	p2.y = (param.height*stretch_scale - p1.y)*pixel_size + param.y_min;
}


void TPoint2Pixel_Origin(TPoint &p1, TPixcelPoint &p2)
{
	p2.x = (int)((p1.x - param.x_min)/param.resolution);
	p2.y = (int)((p1.y - param.y_min)/param.resolution);
}


void Pixel2TPoint_Origin(TPixcelPoint &p1, TPoint &p2)
{
	p2.x = p1.x*param.resolution + param.x_min;
	p2.y = p1.y*param.resolution + param.y_min;
}

void OnGetDeviceMsg(int id, int msgType, int msgCode, void* param)
{
	//LOG("OnGetDeviceMsg %d", msgType);
}

void OnGetRobotMsg(s32 id, s32 Level, s32 Code, char* msg)
{
	LOGW("%s", msg);
}

void RxDataCallBack(int32_t id, void *param, const uint8_t *data, int32_t len)
{
	if (g_SerPortInterface.IsOpened())
	{
		unsigned char buf[10];
		Mpu6500Data* mpuData = (Mpu6500Data*)data;
		int sendLen = GetSendBuf((char*)buf,mpuData->acce_z, mpuData->gyro_z, mpuData->acture_z, mpuData->angular);
		g_SerPortInterface.WriteBuf(buf,sizeof(buf));
	}
}

System::Void MyForm::MyForm_Load(System::Object^  sender, System::EventArgs^  e) 
{
	
}

System::Void MyForm::btn_connect_Click(System::Object^  sender, System::EventArgs^  e)
{
	if (navipackInterfaceId >= 0)
	{
		Destroy(navipackInterfaceId);
	}
	ConnectType conType = SERIAL_CONNECT;
	if (cb_tcp_or_com->CheckState == CheckState::Checked)
	{
		conType = TCP_CONNECT;
	}

	navipackInterfaceId = Create(conType);
	
	std::string _ip;
	int _port;

	if (conType == TCP_CONNECT) 
	{
		MarshalString(textBox_ip_or_com->Text, _ip);
	}
	else
	{
		std::string comPort;
		MarshalString(textBox_ip_or_com->Text, comPort);
		_ip = "\\\\.\\COM";
		_ip += comPort;
	}

	_port = Convert::ToInt32(textBox_port_or_baud_rate->Text, 10);

	cout << "ip:" << _ip << "  port:" << _port << endl;

	if (btn_connect->Text == "Connect")
	{
		btn_connect->Text = "Connecting ...";
		char ipName[100];
		memset(ipName,0,sizeof(ipName));
		strcpy(ipName, _ip.c_str());
	
		if ((cb_connect_mpu->CheckState == CheckState::Checked) && (cb_only_mpu->CheckState == CheckState::Checked))
		{
			gTcpId = g_TCPClientInterface.AddRxCallBack(PT_SERAL_PACKAGE, RxDataCallBack, NULL, 64 * 1024);
			int ret2 = g_TCPClientInterface.Open(ipName, 7777, 0);
			if (ret2 < 0)
			{
				LOGE("open mpu tcp error!");
				btn_connect->Text == "Connect";
			}
			else
			{
				btn_connect->Text = "Disconnect";
			}
			ret2 = g_SerPortInterface.Open("COM2", 9600, 9600);
			if (ret2 < 0)
			{
				LOGE("open g_SerPortInterface error!");
			}
			return;
		}
		else
		{
			int ret = Open(navipackInterfaceId, ipName, 9977);

			if (ret == 0)
			{
				btn_connect->Text = "Disconnect";
				SetCallback(navipackInterfaceId, OnGetDeviceMsg, OnGetRobotMsg, NULL, NULL);

				if (cb_connect_mpu->CheckState == CheckState::Checked)
				{
					gTcpId = g_TCPClientInterface.AddRxCallBack(PT_SERAL_PACKAGE, RxDataCallBack, NULL, 64 * 1024);
					int ret2 = g_TCPClientInterface.Open(ipName, 7777, 0);
					if (ret2 < 0)
					{
						LOGE("open mpu tcp error!");
					}

					ret2 = g_SerPortInterface.Open("COM2", 9600, 9600);
					if (ret2 < 0)
					{
						LOGE("open g_SerPortInterface error!");
					}
				}
			}
			else
			{
				btn_connect->Text == "Connect";
			}
		}
	}
	else
	{
		btn_connect->Text = "Connect";
		if (cb_connect_mpu->CheckState == CheckState::Checked)
		{
			g_TCPClientInterface.RemoveRxCallBack(PT_SERAL_PACKAGE, gTcpId);
		}
	}

}


//建图
System::Void MyForm::btn_build_map_Click(System::Object^  sender, System::EventArgs^  e)
{
	StartMapping(navipackInterfaceId);
}

System::Void MyForm::btn_auto_build_map_Click(System::Object^  sender, System::EventArgs^  e)
{
	StartMapping(navipackInterfaceId,1);
}

System::Void MyForm::btn_save_map_Click(System::Object^  sender, System::EventArgs^  e)
{
	StopMapping(navipackInterfaceId);
}

System::Void MyForm::btn_init_location_Click(System::Object^  sender, System::EventArgs^  e)
{
	InitLocation(navipackInterfaceId);
}

System::Void MyForm::MainTimer_Tick(System::Object^  sender, System::EventArgs^  e) 
{
	drawMap();
}

//绘制显示地图
System::Void MyForm::drawMap()
{
	GetMapLayer(navipackInterfaceId, algMapData, LIDAR_MAP);

	if (!(algMapData->height > 0  && algMapData->width > 0 ))
	{
		//printf("map not ready!\n");
		return;
	}

	stretch_scale = stretch_scale_buf;

	param.height = algMapData->height;
	param.width = algMapData->width;
	param.x_min = algMapData->x_min;
	param.y_min = algMapData->y_min;
	param.resolution = algMapData->resolution;

	pixel_size = param.resolution / stretch_scale;
	paramReady = true;

	int width = param.width;
	int height = param.height;

	static IplImage *origin_img_ = NULL;
	static IplImage *scaled_img_ = NULL;
	delete	pictureBoxMap->Image;

	if (origin_img_)
	{
		cvReleaseImage(&origin_img_);
		cvReleaseImage(&scaled_img_);
	}

	origin_img_ = cvCreateImage(cvSize(width, height), 8, 3);
	scaled_img_ = cvCreateImage(cvSize(width* stretch_scale, height* stretch_scale), 8, 3);

	int totolPixel = 0; int binarPixel = 0;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			int value = algMapData->map[i + (height - j - 1)*width];
			if (value>150 || value < 100)
				totolPixel++;
			if (value < 100)
				binarPixel++;
			origin_img_->imageData[i * 3 + j*origin_img_->widthStep + 0] = algMapData->map[i + (height - j - 1)*width];
			origin_img_->imageData[i * 3 + j*origin_img_->widthStep + 1] = algMapData->map[i + (height - j - 1)*width];
			origin_img_->imageData[i * 3 + j*origin_img_->widthStep + 2] = algMapData->map[i + (height - j - 1)*width];
		}
	}

	//添加虚拟墙
	for (int i = 0; i < virtual_obstacles.size(); i++)
	{
		TPixcelPoint pt;
		TPoint2Pixel_Origin(virtual_obstacles[i], pt);
		origin_img_->imageData[pt.x * 3 + (height - pt.y - 1) *origin_img_->widthStep + 0] = 0;
		origin_img_->imageData[pt.x * 3 + (height - pt.y - 1) *origin_img_->widthStep + 1] = 0;
		origin_img_->imageData[pt.x * 3 + (height - pt.y - 1) *origin_img_->widthStep + 2] = 255;
	}


	System::String^ str = gcnew System::String("");
	str = str->Format("Cover Rate：cover pixels={0}, obstacle pixels={1}, all pixels={2}", totolPixel, binarPixel, width*height);
	toolStripStatusLabel3->Text = str;

	//扩展地图
	cvResize(origin_img_, scaled_img_, CV_INTER_LINEAR);// CV_INTER_NN  CV_INTER_LINEAR);
	drawLidarData(scaled_img_);
	drawMotionTrajectory(scaled_img_);
	drawRobot(scaled_img_);
	drawAstarPoints(scaled_img_);
	pictureBoxMap->Image = gcnew System::Drawing::Bitmap(width* stretch_scale,
		height* stretch_scale,
		scaled_img_->widthStep,
		System::Drawing::Imaging::PixelFormat::Format24bppRgb,
		(System::IntPtr) scaled_img_->imageData);

	if (video)
	{
		IplImage* video_frame = cvCreateImage(CvSize(1280, 960), IPL_DEPTH_8U, 3);
		cvZero(video_frame);
		float mscale = 1;
		if ((float)(scaled_img_->width) / (float)(scaled_img_->height) > 1280.0f / 960.0f)
		{
			mscale = 1280.0f / scaled_img_->width;
		}
		else
		{
			mscale = 960.0f / scaled_img_->height;
		}
		IplImage* tep = cvCreateImage(cvSize(scaled_img_->width*mscale, scaled_img_->height*mscale), 8, 3);
		cvResize(scaled_img_, tep);
		cvSetImageROI(video_frame, cvRect(640 - tep->width / 2, 480 - tep->height / 2, tep->width, tep->height));
		cvCopy(tep, video_frame);
		cvResetImageROI(video_frame);
		cvReleaseImage(&tep);
		int ret = cvWriteFrame(video, video_frame); //判断是否写入成功，如果返回的是1，表示写入成功
		cvReleaseImage(&video_frame);
	}
}

//绘制雷达数据
System::Void MyForm::drawLidarData(IplImage* img)
{
	int cnt = 0; 
	GetSensorData(navipackInterfaceId, algRealLidarData, ST_LIDAR2D);

	TPixcelPoint pp1, pp2;
	float posx = algRealLidarData->posX/1000.0;
	float posy = algRealLidarData->posY/1000.0;
	float posPhi = algRealLidarData->posSita/1000.0;
	for (int i = 0; i < algRealLidarData->num; i++)
	{
		TPoint pt;
		float x = algRealLidarData->localPosX[i] / 1000.0;
		float y = algRealLidarData->localPosY[i] / 1000.0;
		pt.x = x;
		pt.y = y;

		TPoint2Pixel_Scale(pt, pp1);
		cvCircle(img, cvPoint(pp1.x, pp1.y), 1, RED, -1);
	}
}

//绘制机器人人及路径
System::Void MyForm::drawRobot(IplImage* img)
{
	AlgStatusRegister res;
	GetStatus(navipackInterfaceId, &res);
	TPixcelPoint pp;
	TPoint pt;
	pt.x = res.posX / 1000.0;
	pt.y = res.posY / 1000.0;

	addMotionTrajectoryPoint(pt);

	TPoint2Pixel_Scale(pt, pp);
	TPixcelPoint pp_radius1, pp_radius2;
	TPoint pt_radius1, pt_radius2;
	pt_radius1.x = 0.2;
	pt_radius1.y = 0.2;
	pt_radius2.x = 0;
	pt_radius2.y = 0;
	TPoint2Pixel_Scale(pt_radius1, pp_radius1);
	TPoint2Pixel_Scale(pt_radius2, pp_radius2);
	pp_robot_radius = abs(pp_radius1.x - pp_radius2.x);
	cvCircle(img, cvPoint(pp.x, pp.y), pp_robot_radius, BLUE, 2);

	int pose_x = cos(res.posSita / 1000.0) * pp_robot_radius;
	int pose_y = -sin(res.posSita / 1000.0) * pp_robot_radius;
	cvLine(img, cvPoint(pp.x - pose_x, pp.y - pose_y), cvPoint(pp.x + pose_x * 3, pp.y + pose_y * 3), BLUE, 2);
}

//绘制路径规划点

System::Void MyForm::drawAstarPoints(IplImage* img)
{
	int positions_x[MAX_TARGET_LIST_SIZE] = { 0 };
	int positions_y[MAX_TARGET_LIST_SIZE] = { 0 };
	int positions_num = 0;
	GetCurrentPath(navipackInterfaceId, positions_x, positions_y, &positions_num);

	for (int i = 0; i < positions_num; i++)
	{
		TPixcelPoint pp1, pp2;
		TPoint pt1, pt2;
		pt1.x = positions_x[i] / 1000.0;
		pt1.y = positions_y[i] / 1000.0;
		TPoint2Pixel_Scale(pt1, pp1);


		cvCircle(img, cvPoint(pp1.x, pp1.y), pp_robot_radius / 2, SPRINGGREEN, 1);
		if (i == positions_num - 1)
			break;
		pt2.x = positions_x[i + 1] / 1000.0;
		pt2.y = positions_y[i + 1] / 1000.0;
		TPoint2Pixel_Scale(pt2, pp2);
		if (i == positions_num - 2)
		{
			int xStep = (pp2.x - pp1.x) / 10;
			int yStep = (pp2.y - pp1.y) / 10;
			for (int index = 1;index<10; index += 2)
			{
				cvLine(img, cvPoint(pp1.x + xStep*index, pp1.y + yStep*index),
					cvPoint(pp1.x + xStep*(index + 1), pp1.y + yStep*(index + 1)), BLUE, 2);
			}
		}
		else
			cvLine(img, cvPoint(pp1.x, pp1.y), cvPoint(pp2.x, pp2.y), GREEN, 2);
	}

}


//绘制实际轨迹
System::Void MyForm::drawMotionTrajectory(IplImage* img)
{
	vector<TPixcelPoint> motion_trajectory_pixel;
	int size = getMotionTrajectoryPixcel(motion_trajectory_pixel);
	for (int i=0; i<size-1; i++)
	{
		TPixcelPoint pp1 = motion_trajectory_pixel[i];
		TPixcelPoint pp2 = motion_trajectory_pixel[i + 1];
		cvLine(img, cvPoint(pp1.x, pp1.y), cvPoint(pp2.x, pp2.y), CV_RGB(255,255,0), 2);
	}
	
}

System::Void MyForm::textBox_speed_control_key_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
{
	double v_step_size = Convert::ToDouble(textBox_v_step->Text);// 0.1;
	double w_step_size = Convert::ToDouble(textBox_w_step->Text); //0.8;
	textBox_speed_control_key->Text = " ";
	switch (e->KeyChar)
	{
	case 'w':
	case 'W':
		v += v_step_size; printf("key w is pressed! v:%f  w:%f\n", v, w);
		break;
	case 's':
	case 'S':
		v -= v_step_size; printf("key s is pressed! v:%f  w:%f\n", v, w);
		break;
	case 'a':
	case 'A':
		w += w_step_size; printf("key a is pressed! v:%f  w:%f\n", v, w);
		break;
	case 'd':
	case 'D':
		w -= w_step_size;
		printf("key d is pressed! v:%f  w:%f\n", v, w);
		break;
	case ' ':
		v = 0; w = 0;
		printf("key space is pressed! v:%f  w:%f\n", v, w);
		break;
	default:
		break;
	}

	System::String^ str = gcnew System::String("");
	str = str->Format("Speed：v={0}, w={1}", v, w);
	toolStripStatusLabel2->Text = str;
	
}

System::Void MyForm::trackBar_map_scale_Scroll(System::Object^  sender, System::EventArgs^  e)
{
	stretch_scale_buf = trackBar_map_scale->Value;
	if (stretch_scale_buf < 1)stretch_scale_buf = 1;
	textBox_map_scale->Text = trackBar_map_scale->Value.ToString();
}

System::Void MyForm::set_pic_show_scale_Click(System::Object^  sender, System::EventArgs^  e) 
{
	stretch_scale_buf = Convert::ToInt16(textBox_map_scale->Text);
	if (stretch_scale_buf < 1)stretch_scale_buf = 1;
}

System::Void MyForm::pictureBoxMap_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	TPixcelPoint p1;
	p1.x = e->X;
	p1.y = e->Y;

	std::string _str;
	TPoint  p;

	Pixel2TPoint_Scale(p1, p);

	System::String^ str = gcnew System::String("");
	float x = ((int)(p.x * 100))*1.00 / 100;
	float y = ((int)(p.y * 100))*1.00 / 100;
	str = str->Format("Coordinate：x={0}, y={1}", x, y);
	toolStripStatusLabel1->Text = str;

	static TPoint last_pt = TPoint(x, y);
	if (e->Button.Equals(System::Windows::Forms::MouseButtons::Left)
		&&cb_enable_drawing_pen->Checked)
	{
		if (fabs(x-last_pt.x)+fabs(y-last_pt.y)>0.05)
		{
			virtual_obstacles.push_back(TPoint(x,y));
			last_pt = TPoint(x, y);
		}
	}
	
}
System::Void MyForm::pictureBoxMap_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
}

System::Void MyForm::btn_back_to_charge_Click(System::Object^ sender, System::EventArgs^ e)
{
	AutoCharge(navipackInterfaceId);
}

System::Void MyForm::btn_update_navipack_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	UpdateNaviPackFile(navipackInterfaceId, "E:\\workspace\\VS2013\\Win\\navipack_2.0\\bin\\android\\armeabi-v7a\\NaviPack");
}

System::Void MyForm::btn_record_video_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	static bool flag = true;
	static int count = 0;
	char video_name[24];
	sprintf(video_name,"camera_%d.avi",count);
	if (flag)
	{
		btn_record_video->Text = L"Save Video";
		video = cvCreateVideoWriter(video_name,/*-1 */CV_FOURCC('X', 'V', 'I', 'D'), 25,
				cvSize(1280, 960)); //创建CvVideoWriter对象并分配空间  
	}
	else
	{
		btn_record_video->Text = "Record Video";
		cvReleaseVideoWriter(&video);
		video = NULL;
	}
	flag = !flag;
	count++;
}

System::Void MyForm::btn_imu_calibrate_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	ImuCalibrate(navipackInterfaceId);
}

System::Void MyForm::cb_tcp_or_com_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e)
{
	if (cb_tcp_or_com->CheckState == CheckState::Checked)
	{
		textBox_ip_or_com->Text = "192.168.17.1";
		textBox_port_or_baud_rate->Text = "9977";
		label_ip_or_com->Text = "IP";
		label_port_or_baud_rate->Text = "PORT";
	}
	else
	{
		textBox_ip_or_com->Text = "4";
		textBox_port_or_baud_rate->Text = "115200";
		label_ip_or_com->Text = "COM";
		label_port_or_baud_rate->Text = "BAUD RATE";
	}
	return System::Void();
}

System::Void MyForm::cb_connect_mpu_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (cb_connect_mpu->CheckState == CheckState::Checked)
	{
		cb_only_mpu->Enabled = true;
	}
	else
	{
		cb_only_mpu->Enabled = false;
	}
}

System::Void MyForm::btn_update_map_flag_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	static bool map_update_flag = true;
	if (map_update_flag)
	{
		btn_update_map_flag->Text = L"Disable Update Map";
	}
	else
	{
		btn_update_map_flag->Text = "Enable Update Map";
	}
	EnableMapUpdate(navipackInterfaceId, map_update_flag);
	map_update_flag = !map_update_flag;
}

System::Void MyForm::send_speed_timer_tick(System::Object ^ sender, System::EventArgs ^ e)
{
	int send_v = v * 1000;
	int send_w = w * 1000;
	bool stop = send_v == 0 && send_w == 0;
	static bool last_stop = stop;
	if (!stop)
	{
		SetSpeed(navipackInterfaceId, v * 1000, w * 1000);
	}
	else
	{
		if (!last_stop)
		{
			SetSpeed(navipackInterfaceId, 0, 0);
		}
	}
	last_stop = stop;
}

System::Void MyForm::btn_update_lidar_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	UpdateLidarFirmware(navipackInterfaceId);
}

System::Void MyForm::btn_do_clean_task_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	DoCleanTask(navipackInterfaceId);
}

System::Void MyForm::btn_optimize_map_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	static bool enable_flag = false;
	enable_flag = !enable_flag;
	if (enable_flag)
	{
		btn_optimize_map->Text = "Cancel Optimize";
	}
	else
	{
		btn_optimize_map->Text = "Optimize Map";
	}
	
	OptimizeMap(navipackInterfaceId,enable_flag);
}

System::Void MyForm::btn_clear_virtual_obstacles_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	virtual_obstacles.clear();
}

System::Void MyForm::btn_send_virtual_obstacles_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	virtual_wall_map->width = param.width;
	virtual_wall_map->height = param.height;
	virtual_wall_map->x_min = param.x_min;
	virtual_wall_map->y_min = param.y_min;
	virtual_wall_map->resolution = param.resolution;
	memset(virtual_wall_map->map,255, MAX_MAP_SIZE);
	for (int i = 0; i < virtual_obstacles.size(); i++)
	{
		TPixcelPoint pt;
		TPoint2Pixel_Origin(virtual_obstacles[i],pt);
		virtual_wall_map->map[pt.x + pt.y*virtual_wall_map->width] = 0;
	}
	SetMapLayer(navipackInterfaceId, virtual_wall_map, LIDAR_MAP);
}

 System::Void MyForm::btn_send_unified_sensor_data_Click(System::Object^  sender, System::EventArgs^  e)
{
	 UnifiedSensorInfo dataInfo;
	 dataInfo.sensorPosX = 20;
	 dataInfo.sensorPosY = -10;
	 dataInfo.sensorPosPhi = 1000;
	 dataInfo.delayTime = 140;
	 dataInfo.minValidDis = 200;
	 dataInfo.maxValidDis = 4000;
	 dataInfo.sensorType = 0;
	 dataInfo.memoryTime = 30;
	 static int increase = 0;
	 increase = (increase + 2) % 300;
	 for (int i=0; i<360; i++)
	 {
		 dataInfo.detectedData[i] = 300 + i*5 + increase;
	 }
	 
	 SendUnifiedSensor(navipackInterfaceId, dataInfo);
}

 //Cancel Navigation
 System::Void MyForm::pictureBoxMap_MouseDown(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
 {
	 if (e->Button.Equals(System::Windows::Forms::MouseButtons::Right))
	 {
		 SetSpeed(navipackInterfaceId, 0, 0);
	 }
	 if (e->Button.Equals(System::Windows::Forms::MouseButtons::Left))
	 {
		 if (!paramReady)return;

		 if (!cb_enable_drawing_pen->Checked)
		 {
			 //处理选中点
			 TPoint point_click;
			 TPixcelPoint p1;
			 p1.x = e->X;
			 p1.y = e->Y;
			 Pixel2TPoint_Scale(p1, point_click);

			 int px[32], py[32];
			 px[0] = point_click.x * 1000;
			 py[0] = point_click.y * 1000;
			 SetTargets(navipackInterfaceId, px, py, 1, 0);
		 }
		 clearMotionTrajectory();
	 }
 }



#endif