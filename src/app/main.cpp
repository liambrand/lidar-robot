#include <stdbool.h>
#include <stdint.h>
#include <ucos_ii.h>
#include <mbed.h>
#include <SDFileSystem.h>

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
#define	CS		PTE4
#define	DETECT		PTE6

SDFileSystem sd(MOSI, MISO, SCLK, PTE4, "sd");
char buffer[1024];

Serial pc(USBTX, USBRX);
Serial device(D1, D0, 115200);
DigitalOut dtr(D7);
DigitalOut redLed(LED1);

/* MOTOR DRIVER SHIELD */
DigitalOut  M1_DIR(D4);
PwmOut      M1_SPD(D3);

DigitalOut  M2_DIR(D12);
PwmOut      M2_SPD(D11);

DigitalOut  M3_DIR(D8);
PwmOut      M3_SPD(D5);



char startRequest[] = {0xA5, 0x20};
char expressScanRequest[] = {0xA5, 0x82, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22};
char resetRequest[] = {0xA5, 0x40};
char stopRequest[] = {0xA5, 0x25};


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


	
  
  	printf("\nWriting data to the sd card \n");
   	FILE *fp = fopen("/sd/hello.bin", "wb"); // w for write, b for binary
  	 if (fp == NULL) {
       pc.printf("Unable to write the file \n");
   	} 	else {
				//fprintf(fp, "00101010 00101011 11010010 11101010 00101010");
       	fprintf(fp, "00101010 00101011 11010010 11101010 00101010");
       	fclose(fp);
       	pc.printf("File successfully written! \n");
   	}

  	printf("\nReading data from the SD card. \n");
   	fp = fopen("/sd/hello.bin", "rb"); // r for read, b for binary
   	if (fp != NULL) {
       	int size = fread(buffer, sizeof(char), 1024, fp);
       	pc.printf("Number of data read: %d, text from hello.bin file: %s \n", size, buffer);
       	fclose(fp);
   	}

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
  //dtr = 1;
  M1_DIR = 0;
  M2_DIR = 1;
  M3_DIR = 0;

  M1_SPD = 0.0f;
  M2_SPD = 0.0f;
  M3_SPD = 0;

	//device.puts(startRequest);

  /* Task main loop */
  while (true) {
    //pc.printf("appTaskLidar Running!\n");
		//if(device.readable()) {
			//pc.putc(device.getc());
	//}

  OSTimeDlyHMSM(0,0,1,0);
  }
}

