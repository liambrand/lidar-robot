#ifndef __lidar_h__
#define __lidar_h__

#include <mbed.h>
#include <rplidar.h>
#include <BufferedSerial.h>
#include "filewriter.h"

/* LIDAR Variables */
//extern Serial pc(USBTX, USBRX);
extern BufferedSerial lidar_device;
extern RPLidar rplidar;
extern DigitalOut dtr;
extern FileWriter filewriter;

class Lidar{
  public:
    Lidar();
    /* Scanning Behaviour */
	  static void beginScanning(void);
	  static void stopScanning(void);

	  /* Scan Data Writing */
	  //static void takeReadings(float angles[], float distances[]);
	  //static void fakeReadings(void);
};

#endif __lidar_h__
