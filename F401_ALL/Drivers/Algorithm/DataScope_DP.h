#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H

#include "main.h"
#include "dma.h"
#include "usart.h"

/**
 * @brief �������ݽṹ��
*/
typedef struct __Scope_Data_t {
    float Data;
    unsigned char Channel;
}Scope_Data_t;

extern unsigned char DataScope_OutPut_Buffer[42];	                    //������֡���ݻ�����
void DataScope_Get_Channel_Data(float Data,unsigned char Channel);      // дͨ�������� ������֡���ݻ�����
unsigned char DataScope_Data_Generate(unsigned char Channel_Number);    // ����֡�������ɺ��� 
void DataScope_DMA_Send(unsigned char channel_nums);                    // DMA���ͺ���

#endif 





