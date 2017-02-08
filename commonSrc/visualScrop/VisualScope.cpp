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
	CRC_Temp = 0xffff;     //CRC��ʼ��
	for (i = 0; i<8; i++)		//Ҫѭ���Ĵ���������Buf���ֽ���
	{
		CRC_Temp ^= visualData[i];  //���ֽ����
		for (j = 0; j<8; j++)
		{
			if (CRC_Temp & 0x01)  //�����λΪ1
			{
				CRC_Temp = (CRC_Temp >> 1) ^ 0xa001; //��CRC����һλ����0xA001���
			}
			else   //�����λΪ0
			{
				CRC_Temp = CRC_Temp >> 1;  //��CRC����һλ
			}
		}
	}
	visualData[8] = CRC_Temp & 0x00ff;            		   //��9����������CRC�ĵ��ֽ�
	visualData[9] = (CRC_Temp & 0xff00) >> 8;            //����10�����ݸ���CRC�ĸ��ֽ�
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
		CRC_Temp = 0xffff;     //CRC��ʼ��
		for (i=0;i<Len; i++)		//Ҫѭ���Ĵ���������Buf���ֽ���
		{
				CRC_Temp ^= Buf[i];  //���ֽ����
				for (j=0;j<8;j++)
				{
					if (CRC_Temp & 0x01)  //�����λΪ1
					{
						CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001; //��CRC����һλ����0xA001���
					}
					else   //�����λΪ0
					{
						CRC_Temp = CRC_Temp >> 1;  //��CRC����һλ
					 }
				}
		}
		Buf[8] =CRC_Temp & 0x00ff ;            		   //��9����������CRC�ĵ��ֽ�
		Buf[9] = (CRC_Temp & 0xff00) >> 8;            //����10�����ݸ���CRC�ĸ��ֽ�
	}
	*/