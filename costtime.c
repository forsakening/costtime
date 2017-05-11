//create @20170511 by forsakening@sina.cn
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include "costtime.h"

//COSTTIME模块打印函数
#define COSTTIME_LOG_DEBUG(fmt,...) printf("[COSTTIME][DEBUG][%s:%d]"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define COSTTIME_LOG_ERROR(fmt,...) printf("[COSTTIME][ERROR][%s:%d]"fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);

typedef struct
{
	int calcCnt;             //总共需要计算的次数
	int currentCnt;          //当前已经计算的次数
	struct timespec time_s;  //用于记录代码块前时间戳
	struct timespec time_d;  //用于记录代码块后时间戳
	uint64_t max_time;       //用于记录代码块执行的最大时间
	uint64_t min_time;       //用于记录代码块执行的最小时间
	uint64_t leap;           //用于计算当前代码块执行的时间
	uint64_t total;          //用于计算总共的执行时间
	uint64_t avg_time;       //用于计算代码块执行的平均时间，ns 纳秒
	COSTTIME_CB callBack;    //到达统计次数后，执行回调
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

	//计算一些统计信息
	

	//如果大于次数，进行回调
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

