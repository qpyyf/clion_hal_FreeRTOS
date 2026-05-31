#include "FreeRTOS_Demo.h"
/*  freertos 相关的头文件*/
#include "FREERTOS.h"
#include "TASK.h"
/*  需要用到的其他头文件*/
#include "GPIO.h"
#include "stdio.h"
#include "stm32f4xx_it.h"
#include "GPIO.h"

/*启动任务配置*/
#define TASK_START_STACK 128
#define TASK_START_PRIORITY 1
TaskHandle_t Task_Start_Handle;
void Task_Start(void *  pvParameters);

/*任务1配置*/
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t Task1_Handle;
void Task1(void *  pvParameters);

/*任务2配置*/
#define TASK2_STACK 128
#define TASK2_PRIORITY 3
TaskHandle_t Task2_Handle;
void Task2(void *  pvParameters);

/*启动任务3配置*/
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t Task3_Handle;
void Task3(void *  pvParameters);


/**
  * @brief 启动freertos
  */
void FreeRTOS_Start(void)
{
	//进入临界区作用进入临界区的代码不会被打断
	taskENTER_CRITICAL();
	//1，创建一个任务
	xTaskCreate( (TaskFunction_t) Task_Start,//任务函数的地址
                              "Task_Start",//任务函数名字符串
                            TASK_START_STACK ,//任务栈大小，最小128，单位4字节
                            (void *)NULL,//传递给任务的参数
                            TASK_START_PRIORITY,//任务的优先级
                            (TaskHandle_t *) &Task_Start_Handle );//任务句柄的地址
		//2，启动调度器
	vTaskStartScheduler();	
	taskEXIT_CRITICAL();
	//退出临界区
}



void Task_Start(void *  pvParameters){
	
	/*      						创建三个任务*/
	
		xTaskCreate( (TaskFunction_t) Task1,//任务函数的地址
                              "Task1",//任务函数名字符串
                            TASK1_STACK ,//任务栈大小，最小128，单位4字节
                            (void *)NULL,//传递给任务的参数
                            TASK1_PRIORITY,//任务的优先级
                            (TaskHandle_t *) &Task1_Handle );//任务句柄的地址
		xTaskCreate( (TaskFunction_t) Task2,//任务函数的地址
                              "Task2",//任务函数名字符串
                            TASK2_STACK ,//任务栈大小，最小128，单位4字节
                            (void *)NULL,//传递给任务的参数
                            TASK2_PRIORITY,//任务的优先级
                            (TaskHandle_t *) &Task2_Handle );//任务句柄的地址
		xTaskCreate( (TaskFunction_t) Task3,//任务函数的地址
                              "Task3",//任务函数名字符串
                            TASK3_STACK ,//任务栈大小，最小128，单位4字节
                            (void *)NULL,//传递给任务的参数
                            TASK3_PRIORITY,//任务的优先级
                            (TaskHandle_t *) &Task3_Handle );//任务句柄的地址
				/* 删除启动任务自身   启动任务只需调用一次用完就删   */
				vTaskDelete(NULL);
}


/**
  * @brief LED1每500ms闪烁一次
  */
void Task1(void *  pvParameters){
	while(1){
	printf("Task1 运行...\r\n");
	GPIO_Control(LED1,OFF);
	//HAL_Delay(500);
		vTaskDelay(500);
		GPIO_Control(LED1,ON);
	//HAL_Delay(500);
		vTaskDelay(500);
	}
}

/**
  * @brief LED2每500ms闪烁一次
  */
void Task2(void *  pvParameters){
	while(1){
	printf("Task2 运行...\r\n");
	GPIO_Control(LED2,OFF);
	//HAL_Delay(500);
		vTaskDelay(500);
	GPIO_Control(LED2,ON);
	//HAL_Delay(500);
		vTaskDelay(500);
	}
}
/**
  * @brief 判断按键KEY1是否按下，按下则删掉task1。
  */
void Task3(void *  pvParameters){
		while(1){
		//printf("Task3 运行...\r\n");	
		if(HAL_GPIO_ReadPin(key3_GPIO_Port,key3_Pin)==1){
			//KeyNum=0;
				if(Task1_Handle!=NULL)
				{
				printf("删除task1..\r\n");
				vTaskDelete(Task1_Handle);
				Task1_Handle=NULL;
				}
			}
		vTaskDelay(20);
		}
}


