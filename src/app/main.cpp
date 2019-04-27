#include <stdbool.h>
#include <stdint.h>
#include <ucos_ii.h>
#include <mbed.h>
#include <fstream>
#include <string>
#include <SDFileSystem.h>

#include "lidar.h"

/*
*********************************************************************************************************
*                                            APPLICATION TASK PRIORITIES
*********************************************************************************************************
*/

typedef enum {
  MOVEMENT_PRIO = 4,
	SCAN_PRIO,
	WRITE_PRIO
} taskPriorities_t;

/*
*********************************************************************************************************
*                                            APPLICATION TASK STACKS
*********************************************************************************************************
*/

#define  MOVEMENT_STK_SIZE              256
#define  SCAN_STK_SIZE              		256
#define  WRITE_STK_SIZE              		256

static OS_STK movementStk[MOVEMENT_STK_SIZE];
static OS_STK scanStk[SCAN_STK_SIZE];
static OS_STK writeStk[WRITE_STK_SIZE];

/*
*********************************************************************************************************
*                                            APPLICATION FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void appTaskMovement(void *pdata);
static void appTaskScan(void *pdata);
static void appTaskWrite(void *pdata);

/*
*********************************************************************************************************
*                                            GLOBAL TYPES AND VARIABLES 
*********************************************************************************************************
*/

Serial pc(USBTX, USBRX);
Lidar lidar;

/*
*********************************************************************************************************
*                                            GLOBAL FUNCTION DEFINITIONS
*********************************************************************************************************
*/

int main() {

  /* Initialise the OS */
  OSInit();                                                   

  /* Create the tasks */
  OSTaskCreate(appTaskMovement,                               
               (void *)0,
               (OS_STK *)&movementStk[MOVEMENT_STK_SIZE - 1],
               MOVEMENT_PRIO);

  OSTaskCreate(appTaskScan,                               
               (void *)0,
               (OS_STK *)&scanStk[SCAN_STK_SIZE - 1],
               SCAN_PRIO);

	OSTaskCreate(appTaskWrite,                               
               (void *)0,
               (OS_STK *)&writeStk[WRITE_STK_SIZE - 1],
               WRITE_PRIO);

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

static void appTaskMovement(void *pdata) {
  /* Start the OS ticker -- must be done in the highest priority task */
  SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC);

  while (true) {
		pc.printf("hey");
		OSTimeDlyHMSM(0,0,1,0);
    
  }
}

static void appTaskScan(void *pdata) {
	
  //while(true) {
	//	
	//	OSTimeDlyHMSM(0,0,0,4);
	//}
}

static void appTaskWrite(void *pdata) {
	

	//while(true) {
	//	
	//	OSTimeDlyHMSM(0,0,0,5);
	//}
}

