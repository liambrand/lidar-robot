#include <stdbool.h>
#include <stdint.h>
#include <ucos_ii.h>
#include <mbed.h>
#include <SDFileSystem.h>
#include "BufferedSerial.h"
#include "rplidar.h"

/*
*********************************************************************************************************
*                                            APPLICATION TASK PRIORITIES
*********************************************************************************************************
*/

typedef enum {
  LIDAR_PRIO = 4
} taskPriorities_t;

/*
*********************************************************************************************************
*                                            APPLICATION TASK STACKS
*********************************************************************************************************
*/

#define  LIDAR_STK_SIZE              256

static OS_STK lidarStk[LIDAR_STK_SIZE];

/*
*********************************************************************************************************
*                                            APPLICATION FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void appTaskLidar(void *pdata);

/*
*********************************************************************************************************
*                                            GLOBAL TYPES AND VARIABLES 
*********************************************************************************************************
*/

#define MOSI		PTE3
#define MISO		PTE1
#define	SCLK		PTE2
#define	CS		  PTE4
#define	DETECT	PTE6

SDFileSystem sd(MOSI, MISO, SCLK, PTE4, "sd");
char buffer[100];

Serial pc(USBTX, USBRX);
BufferedSerial lidar_device(D1, D0);
RPLidar lidar;
DigitalOut dtr(D7);
DigitalOut redLed(LED1);

struct _rplidar_response_device_health_t deviceHealthInfo;
struct RPLidarMeasurement measurement;

/* MOTOR DRIVER SHIELD */
DigitalOut  M1_DIR(D4);
PwmOut      M1_SPD(D3);

DigitalOut  M2_DIR(D12);
PwmOut      M2_SPD(D11);

DigitalOut  M3_DIR(D8);
PwmOut      M3_SPD(D5);

/* TEMP HACK FOR MOVEMENT DEMONSTRATION */
static void goForward(void);
static void goBackward(void);
static void goLeft(void);
static void goRight(void);

/*
*********************************************************************************************************
*                                            GLOBAL FUNCTION DEFINITIONS
*********************************************************************************************************
*/

int main() {

  /* Initialise the OS */
  OSInit();                                                   

  /* Create the tasks */
  OSTaskCreate(appTaskLidar,                               
               (void *)0,
               (OS_STK *)&lidarStk[LIDAR_STK_SIZE - 1],
               LIDAR_PRIO);

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

static void appTaskLidar(void *pdata) {
  
  /* Start the OS ticker -- must be done in the highest priority task */
  SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC);
  dtr = 1;
  lidar.begin(lidar_device);
  lidar.startScan();
  //lidar.getHealth(deviceHealthInfo);
  //pc.printf("hello");
  //pc.printf(deviceHealthInfo.status + "\n");
  //pc.printf(deviceHealthInfo.error_code + "\n");

  /* Task main loop */
  //pc.printf("\n");
  while (true) {
    lidar.waitPoint();
    measurement = lidar.getCurrentPoint();
    pc.printf("ANGLE: ");
    pc.printf("%f \n", measurement.angle);
    pc.printf("DISTANCE: ");
    pc.printf("%f \n", measurement.distance);

    /*
    printf("\n");
    /*goForward();
    OSTimeDlyHMSM(0,0,1,0);
    goBackward();
    OSTimeDlyHMSM(0,0,1,0);
    goLeft();
    OSTimeDlyHMSM(0,0,1,0);
    goRight();
    OSTimeDlyHMSM(0,0,1,0);*/

    //pc.putc(device.getc());
    //print_byte(device.getc());
    OSTimeDlyHMSM(0,0,0,2);
  }
}



static void goForward() {
  pc.printf("goForward");
  M1_DIR = 1;
  M2_DIR = 1;
  M3_DIR = 1;

  M1_SPD = 0;
  M2_SPD = 0.5f;
  M3_SPD = 0.5f;
}

static void goBackward() {
  pc.printf("goBackward");
  M1_DIR = 1;
  M2_DIR = 0;
  M3_DIR = 0;

  M1_SPD = 0;
  M2_SPD = 0.5f;
  M3_SPD = 0.5f;
}

static void goLeft() {
  pc.printf("goLeft");
  M1_DIR = 0;
  M2_DIR = 0;
  M3_DIR = 1;

  M1_SPD = 0.5f;
  M2_SPD = 0.25f;
  M3_SPD = 0.25f;
}

static void goRight() {
  pc.printf("goRight");
  M1_DIR = 1;
  M2_DIR = 1;
  M3_DIR = 0;

  M1_SPD = 0.5f;
  M2_SPD = 0.25f;
  M3_SPD = 0.25f;
}

