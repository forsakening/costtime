//create @20170511
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include "costtime.h"

//COSTTIMEģ���ӡ����
#define COSTTIME_LOG_DEBUG(fmt,...) printf("[COSTTIME][DEBUG][%s:%d]"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define COSTTIME_LOG_ERROR(fmt,...) printf("[COSTTIME][ERROR][%s:%d]"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);

typedef struct
{
	int calcCnt;             //�ܹ���Ҫ����Ĵ���
	int currentCnt;          //��ǰ�Ѿ�����Ĵ���
	struct timespec time_s;  //���ڼ�¼�����ǰʱ���
	struct timespec time_d;  //���ڼ�¼������ʱ���
	uint64_t max_time;       //���ڼ�¼�����ִ�е����ʱ��
	uint64_t min_time;       //���ڼ�¼�����ִ�е���Сʱ��
	uint64_t leap;           //���ڼ��㵱ǰ�����ִ�е�ʱ��
	uint64_t total;          //���ڼ����ܹ���ִ��ʱ��
	uint64_t avg_time;       //���ڼ�������ִ�е�ƽ��ʱ�䣬ns ����
	COSTTIME_CB callBack;    //����ͳ�ƴ�����ִ�лص�
}COSTTIME_INFO;

static COSTTIME_INFO g_costtime_info[MAX_COSTTIME_MODULE_NUM] = {0};
static int costtime_module_num;

int costtime_init(int moduleID, int calcCnt, COSTTIME_CB callBack)
{
	int i;
	if (NULL == callBack)
	{
		COSTTIME_LOG_ERROR("Callback function NULL! \n");
		return -1;
	}
	
	if (costtime_module_num >= MAX_COSTTIME_MODULE_NUM)
	{
		COSTTIME_LOG_ERROR("Costtime Module Number already more than %d !\n", MAX_COSTTIME_MODULE_NUM);
		return -1;
	}

	if (costtime_module_num > moduleID)
	{
		COSTTIME_LOG_ERROR("moduleID:%d already init!\n", moduleID);
		return -1;
	}

	g_costtime_info[costtime_module_num].moduleID = moduleID;
	g_costtime_info[costtime_module_num].calcCnt = calcCnt;
	g_costtime_info[costtime_module_num].callBack = callBack;

	costtime_module_num++;
	COSTTIME_LOG_DEBUG("Init Module:%d, CalcCnt:%d Ok, Current CosttimeNum:%d !\n", \
						moduleID, calcCnt, costtime_module_num);
	return 0;
}

void costtime_tag_start(int moduleID)
{
	clock_gettime(0, &(g_costtime_info[moduleID].time_s));
	return ;
}

void costtime_tag_end(int moduleID)
{
	clock_gettime(0, &(g_costtime_info[moduleID].time_d));
	g_costtime_info[moduleID].currentCnt++;

	//����һЩͳ����Ϣ
	

	//������ڴ��������лص�
	if (g_costtime_info[moduleID].currentCnt >= g_costtime_info[moduleID].calcCnt)
		g_costtime_info[moduleID].callBack(moduleID);

	return ;
}

void costtime_get_stat(COSTTIME_STAT* stat)
{

}

void costtime_default_callback(int moduleID)
{
	COSTTIME_LOG_DEBUG("=============== Costtime MID:%d Output Start ================== \n", moduleID);
	COSTTIME_LOG_DEBUG("");
	COSTTIME_LOG_DEBUG("");
    COSTTIME_LOG_DEBUG("=============== Costtime MID:%d Output End ================== \n", moduleID);
	
	return ;
}

