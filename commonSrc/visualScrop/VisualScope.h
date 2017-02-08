/*
 * ZY_VisualScope.h
 *
 *  Created on: 2014-10-27
 *      Author: 
 */

#ifndef VISUALSCOPE_H_
#define VISUALSCOPE_H_
#include <inttypes.h>

#pragma pack(push, 1) 
typedef struct _VisualScopeData
{
	float ch1;
	float ch2;
	float ch3;
	float ch4;
}VisualScopeData;
#pragma pack(pop)

int GetSendBuf(char* data, float Channel1, float Channel2, float Channel3, float Channel4);
//void VisualScope_Send(float Channel1,float Channel2,float Channel3,float Channel4);//œ‘ æ∑∂Œß -30000~30000;
//void CRC_CHECK(uint8_t *Buf, uint8_t Len);
#endif /* ZY_VISUALSCOPE_H_ */
