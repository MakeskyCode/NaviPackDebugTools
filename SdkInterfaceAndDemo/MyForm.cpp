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
void TPoint2Pixel(TPoint &p1, TPixcelPoint &p2)
{
	p2.x = (int)((p1.x - param.x_min) / pixel_size);
	p2.y = param.height*stretch_scale - (int)((p1.y - param.y_min) / pixel_size);
}

/** \brief 像素坐标转为世纪座标 */
void Pixel2TPoint(TPixcelPoint &p1, TPoint &p2)
{
	p2.x = p1.x*pixel_size + param.x_min;
	p2.y = (param.height*stretch_scale - p1.y)*pixel_size + param.y_min;
}

void OnGetDeviceMsg(int id, int msgType, int msgCode, void* param)
{
	LOG("OnGetDeviceMsg %d", msgType);
}

void OnGetErrorMsg(s32 id, s32 errorLevel, s32 errorCode, char* msg)
{
	LOG("OnGetErrorMsg %s", msg);
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

System::Void MyForm::button_connect_Click(System::Object^  sender, System::EventArgs^  e)
{
	if (navipackInterfaceId >= 0)
	{
		Destroy(navipackInterfaceId);
	}
	ConnectType conType = SERIAL_CONNECT;
	if (CB_UseTcp->CheckState == CheckState::Checked)
	{
		conType = TCP_CONNECT;
	}

	navipackInterfaceId = Create(conType);
	
	std::string _ip;
	int _port;

	if (conType == TCP_CONNECT) 
	{
		MarshalString(tb_ip->Text, _ip);
	}
	else
	{
		std::string comPort;
		MarshalString(tb_ip->Text, comPort);
		_ip = "\\\\.\\COM";
		_ip += comPort;
	}

	_port = Convert::ToInt32(tb_port->Text, 10);

	cout << "ip:" << _ip << "  port:" << _port << endl;

	if (button_connect->Text == "Connect")
	{
		button_connect->Text = "Connecting...";
		char ipName[100];
		memset(ipName,0,sizeof(ipName));
		strcpy(ipName, _ip.c_str());
	
		if ((cb_connectMpu->CheckState == CheckState::Checked) && (CB_ONLY_MPU->CheckState == CheckState::Checked))
		{
			gTcpId = g_TCPClientInterface.AddRxCallBack(PT_SERAL_PACKAGE, RxDataCallBack, NULL, 64 * 1024);
			int ret2 = g_TCPClientInterface.Open(ipName, 7777, 0);
			if (ret2 < 0)
			{
				LOGE("open mpu tcp error!");
				button_connect->Text == "Connect";
			}
			else
			{
				button_connect->Text = "Disconnect";
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
			int ret = Open(navipackInterfaceId, ipName, 9977);// naviInterface->Connect(_ip);

			if (ret == 0)
			{
				button_connect->Text = "Disconnect";
				SetCallback(navipackInterfaceId, OnGetDeviceMsg, OnGetErrorMsg, NULL, NULL);

				if (cb_connectMpu->CheckState == CheckState::Checked)//选中状态，则顺便开启MPU的数据
				{
					gTcpId = g_TCPClientInterface.AddRxCallBack(PT_SERAL_PACKAGE, RxDataCallBack, NULL, 64 * 1024);
					int ret2 = g_TCPClientInterface.Open(ipName, 7777, 0);
					if (ret2 < 0)
					{
						LOGE("Open mpu tcp error!");
					}

					ret2 = g_SerPortInterface.Open("COM2", 9600, 9600);
					if (ret2 < 0)
					{
						LOGE("Open g_SerPortInterface error!");
					}
				}
			}
			else
			{
				button_connect->Text == "Connect";
			}
		}
	}
	else
	{
		button_connect->Text = "Connect";
		if (cb_connectMpu->CheckState == CheckState::Checked)//选中状态，则顺便开启MPU的数据
		{
			g_TCPClientInterface.RemoveRxCallBack(PT_SERAL_PACKAGE, gTcpId);
		}
	}

}


//建图
System::Void MyForm::button_start_build_map_Click(System::Object^  sender, System::EventArgs^  e)
{
	StartMapping(navipackInterfaceId);
}

System::Void MyForm::button_start_build_map_auto_Click(System::Object^  sender, System::EventArgs^  e)
{
	StartMapping(navipackInterfaceId,1);
}

System::Void MyForm::button_stop_build_map_Click(System::Object^  sender, System::EventArgs^  e)
{
	StopMapping(navipackInterfaceId);
}

System::Void MyForm::button_init_map_Click(System::Object^  sender, System::EventArgs^  e)
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
	System::String^ str = gcnew System::String("");
	str = str->Format("覆盖度：遍历的像素={0},障碍物像素={1}, 总像素={2}", totolPixel, binarPixel, width*height);
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
		pt.x = x;//posx + x*cos(posPhi) - y*sin(posPhi);
		pt.y = y;//posy + x*sin(posPhi) + y*cos(posPhi);

		TPoint2Pixel(pt, pp1);
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

	addMotionTrajectoryPoint(pt);//记录当前点位置，用于显示轨迹

	TPoint2Pixel(pt, pp);
	TPixcelPoint pp_radius1, pp_radius2;
	TPoint pt_radius1, pt_radius2;
	pt_radius1.x = 0.2;
	pt_radius1.y = 0.2;
	pt_radius2.x = 0;
	pt_radius2.y = 0;
	TPoint2Pixel(pt_radius1, pp_radius1);
	TPoint2Pixel(pt_radius2, pp_radius2);
	pp_robot_radius = abs(pp_radius1.x - pp_radius2.x);
	cvCircle(img, cvPoint(pp.x, pp.y), pp_robot_radius, BLUE, 2);//机器人位置

	int pose_x = cos(res.posSita / 1000.0) * pp_robot_radius;
	int pose_y = -sin(res.posSita / 1000.0) * pp_robot_radius;
	cvLine(img, cvPoint(pp.x - pose_x, pp.y - pose_y), cvPoint(pp.x + pose_x * 3, pp.y + pose_y * 3), BLUE, 2);
	//机器人方向

	//画gridmap
	//float grid_unit = 0.1;
	//for (int i = -50; i < 50; i++)
	//{
	//	CvScalar color;
	//	if (i % 10 == 0)
	//		color = CV_RGB(100, 100, 100);
	//	else
	//		color = CV_RGB(200, 200, 200);
	//	TPixcelPoint pp1, pp2;
	//	TPoint pt1, pt2;
	//	pt1.x = i*grid_unit + pt.x;
	//	pt1.y = -50 + pt.y;
	//	pt2.x = i*grid_unit + pt.x;;
	//	pt2.y = 50 + pt.y;
	//	TPoint2Pixel(pt1, pp1);
	//	TPoint2Pixel(pt2, pp2);
	//	cvLine(img, cvPoint(pp1.x, pp1.y), cvPoint(pp2.x, pp2.y), color, 1);

	//	pt1.y = i*grid_unit + pt.y;
	//	pt1.x = -50 + pt.x;
	//	pt2.y = i*grid_unit + pt.y;
	//	pt2.x = 50 + pt.x;
	//	TPoint2Pixel(pt1, pp1);
	//	TPoint2Pixel(pt2, pp2);
	//	cvLine(img, cvPoint(pp1.x, pp1.y), cvPoint(pp2.x, pp2.y), color, 1);

	//}
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
		TPoint2Pixel(pt1, pp1);


		cvCircle(img, cvPoint(pp1.x, pp1.y), pp_robot_radius / 2, SPRINGGREEN, 1);
		if (i == positions_num - 1)
			break;
		pt2.x = positions_x[i + 1] / 1000.0;
		pt2.y = positions_y[i + 1] / 1000.0;
		TPoint2Pixel(pt2, pp2);
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

System::Void MyForm::button_send_target_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//SdkProtocolHeader head;
	//head.deviceAddr = ALG_ADDRESS;
	//head.functionCode = ALG_CONTROL_REG;
	//u8 data[5] = {1,2,3,4,5};
	////u8 *data = "sasss";
	//navi_interface->mNetworkInterface->WriteData(PT_SERAL_PACKAGE, (u8*)&head, sizeof(SdkProtocolHeader));
}

System::Void MyForm::textBox_key_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
{
	double v_step_size = Convert::ToDouble(textBox_v_step->Text);// 0.1;
	double w_step_size = Convert::ToDouble(textBox_w_step->Text); //0.8;
	textBox_key->Text = " ";
	switch (e->KeyChar)
	{
	case 'w':
		v += v_step_size; printf("key w is pressed! v:%f  w:%f\n", v, w);
		break;
	case 's':
		v -= v_step_size; printf("key s is pressed! v:%f  w:%f\n", v, w);
		break;
	case 'a':
		w += w_step_size; printf("key a is pressed! v:%f  w:%f\n", v, w);
		break;
	case 'd':
		w -= w_step_size;
		printf("key d is pressed! v:%f  w:%f\n", v, w);
		break;
	case ' ':
		v = 0; w = 0;
		printf("key blank is pressed! v:%f  w:%f\n", v, w);
		break;
	default:
		break;
	}

	System::String^ str = gcnew System::String("");
	str = str->Format("speed：v={0}, w={1}", v, w);
	toolStripStatusLabel2->Text = str;
	
}

//设置地图显示比例
System::Void MyForm::trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e)
{
	stretch_scale_buf = trackBar1->Value;
	if (stretch_scale_buf < 1)stretch_scale_buf = 1;
	pic_scale->Text = trackBar1->Value.ToString();
}

System::Void MyForm::set_pic_show_scale_Click(System::Object^  sender, System::EventArgs^  e) 
{
	stretch_scale_buf = Convert::ToInt16(pic_scale->Text);
	if (stretch_scale_buf < 1)stretch_scale_buf = 1;
}

System::Void MyForm::pictureBoxMap_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	////处理选中点
	//TPoint point_click;
	//TPixcelPoint p1;
	//p1.x = e->X;
	//p1.y = e->Y;
	//navi_interface->Pixel2TPoint(p1, point_click);
	////TTargetPoint tarPoint;
	//navi_interface->targetPoint.pt = point_click;
	//navi_interface->targetPoint.phi = -1;

}
System::Void MyForm::pictureBoxMap_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	TPixcelPoint p1;
	p1.x = e->X;
	p1.y = e->Y;

	std::string _str;
	TPoint  p;

	Pixel2TPoint(p1, p);

	System::String^ str = gcnew System::String("");
	float x = ((int)(p.x * 100))*1.00 / 100;
	float y = ((int)(p.y * 100))*1.00 / 100;
	str = str->Format("mouse coodinate：x={0}, y={1}", x, y);
	toolStripStatusLabel1->Text = str;

}
System::Void MyForm::pictureBoxMap_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{

	if (!paramReady)return;

	//处理选中点
	TPoint point_click;
	TPixcelPoint p1;
	p1.x = e->X;
	p1.y = e->Y;
	Pixel2TPoint(p1, point_click);

	int px[32], py[32];
	px[0] = point_click.x * 1000;
	py[0] = point_click.y * 1000;
	SetTargets(navipackInterfaceId,px,py,1,0);

	clearMotionTrajectory();
}

System::Void MyForm::checkBox_simulation_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
//	ValidateSimulationOrRealMode();
}


System::Void MyForm::button_config_Click(System::Object^  sender, System::EventArgs^  e)
{

}

System::Void MyForm::button_save_simplemap_Click(System::Object^  sender, System::EventArgs^  e)
{
//// 	CCriticalSectionLocker locker(&navi_interface->tcp_send_cs_);
//	TMapSensorInfo *map_save_info = &(navi_interface->mMapManagerAction.map_save_info);
//	//刷新bits
//	map_save_info->data = 0;
//	map_save_info->bits.lidar_map = checkBox_lidar_map->Checked;
//	map_save_info->bits.rgbd_map = checkBox_rgbd_map->Checked;
//	map_save_info->bits.collision_map = checkBox_collision_map->Checked;
//	map_save_info->bits.ground_map = checkBox_ground_map->Checked;
//	map_save_info->bits.ultrasonic_map = checkBox_ultrasonic_map->Checked;
//
//	int tmp_num = Convert::ToInt16(textBox_map_file_dir_num->Text);
//	if (tmp_num >= 0 && tmp_num < MAX_MAP_DIR_NUM)
//	{
//		navi_interface->mMapManagerAction.map_file_num = tmp_num;
//	}
//	SdkProtocolHeader head;
//	u8 *buf = new u8[5000];
//	memset(buf, 0, 5000);
//	int data_len = 0;
//	head.deviceAddr = ALG_ADDRESS;
//	head.functionCode = ALG_SAVE_MAP;
//	head.startAddr = 0;
//	data_len = 0;
//	memcpy(buf, (u8*)&head, sizeof(SdkProtocolHeader)); data_len += sizeof(SdkProtocolHeader);
//	memcpy(buf + data_len, (u8*)&navi_interface->mMapManagerAction, sizeof(TMapManagerAction)); 
//	data_len += sizeof(TMapManagerAction);
//	navi_interface->mNetworkInterface->WriteData(PT_SERAL_PACKAGE, buf, data_len);
//	delete buf;

}

System::Void MyForm::back_charge_Click(System::Object^ sender, System::EventArgs^ e)
{
	AutoCharge(navipackInterfaceId);
}

System::Void MyForm::button1_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	UpdateNaviPackFile(navipackInterfaceId, "E:\\workspace\\VS2013\\Win\\navipack_2.0\\bin\\android\\armeabi-v7a\\NaviPack");
}

System::Void MyForm::button_video_record_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	static bool flag = true;
	static int count = 0;
	char video_name[24];
	sprintf(video_name,"camera_%d.avi",count);
	if (flag)
	{
		button_video_record->Text = L"stop recording video";
		video = cvCreateVideoWriter(video_name,/*-1 */CV_FOURCC('X', 'V', 'I', 'D'), 25,
				cvSize(1280, 960)); //创建CvVideoWriter对象并分配空间  
	}
	else
	{
		button_video_record->Text = "start recording video";
		cvReleaseVideoWriter(&video);
		video = NULL;
	}
	flag = !flag;
	count++;
}

System::Void MyForm::button_IMU_calibrate_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	ImuCalibrate(navipackInterfaceId);
}

System::Void MyForm::CB_UseTcp_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e)
{
	if (CB_UseTcp->CheckState == CheckState::Checked)
	{
		tb_ip->Text = "192.168.20.173";
		tb_port->Text = "9977";
	}
	else
	{
		tb_ip->Text = "4";
		tb_port->Text = "115200";
	}
	return System::Void();
}

System::Void MyForm::cb_connectMpu_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (cb_connectMpu->CheckState == CheckState::Checked)
	{
		CB_ONLY_MPU->Enabled = true;
	}
	else
	{
		CB_ONLY_MPU->Enabled = false;
	}
}

System::Void MyForm::btn_update_map_flag_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	static bool map_update_flag = true;
	if (map_update_flag)
	{
		btn_update_map_flag->Text = L"Disable Map Upgrade";
	}
	else
	{
		btn_update_map_flag->Text = "Enable Map Upgrade";
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

 System::Void MyForm::BTN_UnifiedSensor_Click(System::Object^  sender, System::EventArgs^  e)
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



#endif