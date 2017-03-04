/**
 * @file        pro.h
 * @author      陈维
 * @version     V01
 * @date        2016.09.21
 * @brief       协议定义
 * @note
 *
 * @attention   COYPRIGHT INMOTION ROBOT
 **/

#ifndef _PRO_H_
#define _PRO_H_

#define u8               unsigned char
#define u16              unsigned short
#define u32              unsigned int
#define s16              short
#define LIDAR_ADDRESS    0x10
#define PARSE_LEN           8192    //>1036
#define MIN_PRO_NUM      14

#pragma pack(push,1)
typedef enum
{
    PACK_FAIL,
    PACK_OK
} ResultTypeDef;

#define NULL    0


typedef struct
{
    u8  DeviceAddr;   
    u8  FunctionCode; 
    u16 StartAddr;    
    u32 Len;
} SdkProtocolHeaderTypeDef;


//数据包头尾、控制字
#define P_HEADER     0xAA
#define P_TAIL       0x55
#define P_CTRL       0xA5
#define P_FAIL       0
#define P_SUCCESS    1



/**
* @brief  数据包ID
*/
typedef enum {
	PACK_LIDAR_DATA,
	PACK_SET_PIXOFFSET,      /*!< 校准像素偏移量  */
	PACK_FLASH_CONFIG,       /*!< 烧录配置到flash  */
	PACK_GET_GYROSCOPE_DATA, /*!< 获取陀螺仪数据 */
	PACK_SET_SPEED,          /*!< 设置Lidar速度 */
	PACK_FIRMWARE_UPDATE,    /*!< 更新Firmware */
	PACK_ANGLE_OFFSET,       /*!< 调整角度偏移量*/
	PACK_CONTROL_LASER,      /*!< 控制激光*/
	PACK_DEBUG_MODE,         /*!< 调试模式*/
	PACK_START_ROTATE,       /*!< 开始旋转 */
	PACK_COM_TEST,           /*!< 通讯测试模式*/
	PACK_FORCE_LASER_OPEN,   /*!< 强制激光输出*/
	PACK_OPEN_LASER_DEFAULT,
	PACK_CONFIDENCE_FILTER,
	PACK_SET_LASER_MODULATE_PERIOD,
	PACK_ACK = 0xfd,
	PACK_SELF_TEST = 0xfe, /*!< 自检模式 */
	PACK_NULL = 0xff       /*!< 复位值，表明当前没有数据包 */
} PackageIDTypeDef;

typedef enum {
	FW_UPDATE_READY,
	FW_CRC_ERROR,
	FW_FLASH_ERROR,
	FW_OK,
	FW_NULL = 0xff,
} FirmwareUpdateType;

typedef struct
{
    PackageIDTypeDef DataID;
    u8               *DataInBuff;
    u32              DataInLen;
    u8               *DataOutBuff;
    u32              *DataOutLen;
} PackageDataStruct;


typedef struct IMUDataToLidar {
	SdkProtocolHeader header;
	short gyro_z;

	IMUDataToLidar() {
		header.deviceAddr = LIDAR_ADDRESS;	//  cmd
		header.functionCode = PACK_GET_GYROSCOPE_DATA;	// sub cmd
		header.startAddr = 0;		//sub sub cmd
		header.len = 2;
	}
}IMUDataToLidar;

typedef struct UpdateCmdToLidar {
	SdkProtocolHeader header;
	char update_data[2048];

	UpdateCmdToLidar() {
		header.deviceAddr = LIDAR_ADDRESS;	//  cmd
		header.functionCode = PACK_FIRMWARE_UPDATE;	// sub cmd
		header.startAddr = 0;		//sub sub cmd
		header.len = 2;
	}
}UpdateCmdToLidar;

#pragma pack(pop)
#endif



/************************ (C) COPYRIGHT INMOTION ROBOT *****END OF FILE****/
