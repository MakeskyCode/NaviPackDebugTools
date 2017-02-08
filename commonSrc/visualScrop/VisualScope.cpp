/*
 * ZY_VisualScope.c
 *
 *  Created on: 2014-10-27
 *      Author: Yue
 */

#include "VisualScope.h"
#include <stdio.h>
#include <string.h>

static float getShortFromFloat(float val)
{
	int r;
	val > 32767 ? r = 32767 : (val < -32767 ? r = -32767 : r = val);
	return r;
}

int GetSendBuf(char* data, float Channel1, float Channel2, float Channel3, float Channel4)
{

	

	uint8_t visualData[10];
	visualData[0] = ((uint16_t)(getShortFromFloat(Channel1))) & 0x00ff;
	visualData[1] = ((uint16_t)(getShortFromFloat(Channel1)) & 0xff00) >> 8;
	visualData[2] = ((uint16_t)(getShortFromFloat(Channel2))) & 0x00ff;
	visualData[3] = ((uint16_t)(getShortFromFloat(Channel2)) & 0xff00) >> 8;
	visualData[4] = ((uint16_t)(getShortFromFloat(Channel3))) & 0x00ff;
	visualData[5] = ((uint16_t)(getShortFromFloat(Channel3)) & 0xff00) >> 8;
	visualData[6] = ((uint16_t)(getShortFromFloat(Channel4))) & 0x00ff;
	visualData[7] = ((uint16_t)(getShortFromFloat(Channel4)) & 0xff00) >> 8;

	uint16_t CRC_Temp;
	uint8_t i, j, k;
	CRC_Temp = 0xffff;     //CRC初始化
	for (i = 0; i<8; i++)		//要循环的次数，就是Buf的字节数
	{
		CRC_Temp ^= visualData[i];  //逐字节异或
		for (j = 0; j<8; j++)
		{
			if (CRC_Temp & 0x01)  //如果低位为1
			{
				CRC_Temp = (CRC_Temp >> 1) ^ 0xa001; //将CRC右移一位后与0xA001异或
			}
			else   //如果低位为0
			{
				CRC_Temp = CRC_Temp >> 1;  //将CRC右移一位
			}
		}
	}
	visualData[8] = CRC_Temp & 0x00ff;            		   //第9个数据上是CRC的低字节
	visualData[9] = (CRC_Temp & 0xff00) >> 8;            //将第10个数据赋上CRC的高字节
	memcpy(data,visualData,10);
	return 10;
}
/*
uint8_t ZY_VisualScope[10] = { 0x00,0x01,0x00,0x2,0x00,0x03,0x0,0x4,0x00,0x00 };
// -30000~30000;
void VisualScope_Send(float Channel1,float Channel2,float Channel3,float Channel4)
{
			ZY_VisualScope[0]=((uint16_t)(Channel1)) & 0x00ff;
			ZY_VisualScope[1]=((uint16_t)(Channel1) & 0xff00) >> 8;
			ZY_VisualScope[2]=((uint16_t)(Channel2)) & 0x00ff;
			ZY_VisualScope[3]=((uint16_t)(Channel2) & 0xff00) >> 8;
			ZY_VisualScope[4]=((uint16_t)(Channel3)) & 0x00ff;
			ZY_VisualScope[5]=((uint16_t)(Channel3) & 0xff00) >> 8;
			ZY_VisualScope[6]=((uint16_t)(Channel4)) & 0x00ff;
			ZY_VisualScope[7]=((uint16_t)(Channel4) & 0xff00) >> 8;


			

}

void CRC_CHECK(uint8_t *Buf, uint8_t Len)
{
		uint16_t CRC_Temp;
		uint8_t i,j,k;
		CRC_Temp = 0xffff;     //CRC初始化
		for (i=0;i<Len; i++)		//要循环的次数，就是Buf的字节数
		{
				CRC_Temp ^= Buf[i];  //逐字节异或
				for (j=0;j<8;j++)
				{
					if (CRC_Temp & 0x01)  //如果低位为1
					{
						CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001; //将CRC右移一位后与0xA001异或
					}
					else   //如果低位为0
					{
						CRC_Temp = CRC_Temp >> 1;  //将CRC右移一位
					 }
				}
		}
		Buf[8] =CRC_Temp & 0x00ff ;            		   //第9个数据上是CRC的低字节
		Buf[9] = (CRC_Temp & 0xff00) >> 8;            //将第10个数据赋上CRC的高字节
	}
	*/