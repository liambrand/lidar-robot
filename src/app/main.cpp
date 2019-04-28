#include <stdbool.h>
#include <stdint.h>
#include <ucos_ii.h>
#include <mbed.h>
#include <fstream>
#include <string>
#include <SDFileSystem.h>

#include "lidar.h"
#include "control.h"

/*
*********************************************************************************************************
*                                            APPLICATION TASK PRIORITIES
*********************************************************************************************************
*/

typedef enum {
  ROBOT_PRIO = 4,
} taskPriorities_t;

/*
*********************************************************************************************************
*                                            APPLICATION TASK STACKS
*********************************************************************************************************
*/

#define  ROBOT_STK_SIZE              256

static OS_STK robotStk[ROBOT_STK_SIZE];

/*
*********************************************************************************************************
*                                            APPLICATION FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void appTaskRobot(void *pdata);

/*
*********************************************************************************************************
*                                            GLOBAL TYPES AND VARIABLES 
*********************************************************************************************************
*/

Serial pc(USBTX, USBRX);
Lidar lidar;
Control control;

/*
*********************************************************************************************************
*                                            GLOBAL FUNCTION DEFINITIONS
*********************************************************************************************************
*/

int main() {

  /* Initialise the OS */
  OSInit();                                                   

  /* Create the tasks */
  OSTaskCreate(appTaskRobot,                               
               (void *)0,
               (OS_STK *)&robotStk[ROBOT_STK_SIZE - 1],
               ROBOT_PRIO);

  /* Start the OS */
  OSStart();                                                  
  
  /* Should never arrive here */ 
  return 0;      
}

/*
*********************************************************************************************************
*                                            APPLICATION TASK DEFINITIONS
*********************************************************************************************************
*/

static void appTaskRobot(void *pdata) {
  /* Start the OS ticker -- must be done in the highest priority task */
  SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC);

  while (true) {
		zpc.printf("hey");
		OSTimeDlyHMSM(0,0,1,0);
    
  }
}

