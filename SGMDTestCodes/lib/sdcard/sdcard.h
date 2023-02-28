#include "SD.h"
#include "FS.h"

void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void testFileIO(fs::FS &fs, const char * path);
void appendFile(fs::FS &fs, const char * path, const char * message);