#include "lidar.h"

DigitalOut dtr(D7);
RPLidar rplidar;
BufferedSerial lidar_device(D1, D0, 115200);

Lidar::Lidar() {
	dtr = 0;
	rplidar.begin(lidar_device);
  rplidar.startScan();
}

static void beginScanning() {
  dtr = 1;
}

static void stopScanning() {
  dtr = 0;
}


