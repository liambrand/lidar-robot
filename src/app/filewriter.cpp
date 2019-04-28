#include "filewriter.h"

#define MOSI		PTE3
#define MISO		PTE1
#define	SCLK		PTE2
#define	CS		  PTE4
SDFileSystem sd(MOSI, MISO, SCLK, CS, "sd");

FileWriter::FileWriter() {
	
}

void writeReadings(float angles[], float measurements[]) {
  // method stub
}

