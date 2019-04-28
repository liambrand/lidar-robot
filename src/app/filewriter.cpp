#include "filewriter.h"

#define MOSI		PTE3
#define MISO		PTE1
#define	SCLK		PTE2
#define	CS		  PTE4
SDFileSystem sd(MOSI, MISO, SCLK, CS, "sd");

FileWriter::FileWriter() {
	
}

void writeReadings(float angles[], float distances[]) {
  int arraySize = sizeof(readingsBuffer)/sizeof(float);
  
	// Create the readings file on the Micro SD-Card
	FILE *fp = fopen("/sd/readings.txt", "w");

	// Error checking
  if (fp == NULL) {
    pc.printf("Unable to access/create file \n");
  }

	// Iterate through the readingsBuffer and write the angle/distance pairs
	for(int i = 0; i < arraySize; i++) {
		fprintf(fp, "%f %f", angles[i], distances[i]);
	}

	// Close file
fclose(fp);
}

