#include <stdbool.h>
#include <stdint.h>
#include <ucos_ii.h>
#include <mbed.h>
#include <fstream>
#include <string>
#include <SDFileSystem.h>
#include "BufferedSerial.h"
#include <rplidar.h>

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
#define  SCAN_STK_SIZE              	256
#define  WRITE_STK_SIZE              	256

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

/* Serial for terminal output */
Serial pc(USBTX, USBRX);

/* File System Variables */
#define MOSI		PTE3
#define MISO		PTE1
#define	SCLK		PTE2
#define	CS		  	PTE4
SDFileSystem sd(MOSI, MISO, SCLK, CS, "sd");

/* LIDAR Variables */
BufferedSerial lidar_device(D1, D0);
RPLidar lidar;
DigitalOut dtr(D7);
DigitalOut redLed(LED1);

/* Motor Driver Shield */
DigitalOut  M1_DIR(D4);
PwmOut      M1_SPD(D3);

DigitalOut  M2_DIR(D12);
PwmOut      M2_SPD(D11);

DigitalOut  M3_DIR(D8);
PwmOut      M3_SPD(D5);

/* Buffer */
float readingsBuffer[16000][2];

// Semaphores
OS_EVENT *readyToScan;
OS_EVENT *readyToWrite;

/* Scanning methods */
static void beginScanning(void);
static void stopScanning(void);
static void takeReadings(void);
static void writeReadings(void);

/* Movement methods */
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

	/* Declare semaphores */
	readyToScan = OSSemCreate(0);
	readyToWrite = OSSemCreate(0);

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
	uint8_t status;

	// Initialize LIDAR
	dtr = 0;
	lidar.begin(lidar_device);
	lidar.startScan();

  while (true) {
		// Movement session
		pc.printf("Moving - First...\n");
		OSTimeDlyHMSM(0,0,6,0); // how long it moves for
		pc.printf("Movement stopped.\n ");
		status = OSSemPost(readyToScan);
		OSSemPend(readyToScan, 0, &status);
		// Stop moving and write data to file
		status = OSSemPost(readyToWrite);
		OSSemPend(readyToWrite, 0, &status);
		
		OSTimeDlyHMSM(0,0,0,5);
  }
}


static void appTaskScan(void *pdata) {
	uint8_t status;
	
	while(true) {
		OSSemPend(readyToScan, 0, &status);
		pc.printf("Scanning...\n");
		beginScanning();
		takeReadings();
		stopScanning();
		pc.printf("Stopping scan.\n");
		status = OSSemPost(readyToScan);
		OSTimeDlyHMSM(0,0,0,4);
	}
}


static void appTaskWrite(void *pdata) {
	uint8_t status;
	// Have to half the calculated array size due to its 2 dimensional nature
	int arraySize = (sizeof(readingsBuffer)/sizeof(float))/2;

	while(true) {
		OSSemPend(readyToWrite, 0, &status);
		pc.printf("Writing readings to SD card...\n");
		writeReadings();
		pc.printf("Done writing.\n");
		status = OSSemPost(readyToWrite);
		pc.printf("Releasing write semaphore...");
		OSTimeDlyHMSM(0,0,0,5);
	}
}


static void beginScanning() {
  dtr = 1;
}


/**
*	Write the stored LIDAR readings to the Micro-SD Card
*/
static void writeReadings() {
	int arraySize = (sizeof(readingsBuffer)/sizeof(float))/2;
  
	// Create the readings file on the Micro SD-Card
	FILE *fp = fopen("/sd/readings.txt", "w");

	// Log if the file cannot be made
	if (fp == NULL) {
		pc.printf("Unable to access/create file \n");
	}

	// Iterate through the readingsBuffer and write the angle/distance pairs
	for(int i = 0; i < arraySize; i++) {
		fprintf(fp, "%f %f\r\n", readingsBuffer[i][0], readingsBuffer[i][1]);
	}

	// Close file
	fclose(fp);
}

/*
*	Begin storing readings in the program's buffer
*/
static void takeReadings() {
	struct RPLidarMeasurement measurement;
	// To prevent constantly calculating the size of the array, we store it as a local variable
	int arraySize = (sizeof(readingsBuffer)/sizeof(float))/2;
	for(int i = 0; i < arraySize; i++) {
			lidar.waitPoint();
  		measurement = lidar.getCurrentPoint();
			// Get angle
			readingsBuffer[i][0] = measurement.angle;

			// Get distance
			readingsBuffer[i][1] = measurement.distance;
	}
}

/*
*	Set the DTR to LOW
*/
static void stopScanning() {
  dtr = 0;
}


/*
*	Hardcoded directional drive
*/

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

