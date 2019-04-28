#ifndef __filewriter_h__
#define __filewriter_h__

#include <SDFileSystem.h>

/* SDFileSystem Variables */
extern SDFileSystem sd;

class FileWriter{ 
    public:
      FileWriter();
      void writeReadings(float angles[], float measurements[]);
    
};

#endif __filewriter_h__
