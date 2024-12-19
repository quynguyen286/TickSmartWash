#ifndef SD_CARD_H
#define SD_CARD_H
#include "global.h"
void writeFile(fs::FS &fs, const char *path, const char *message);
void readFile(fs::FS &fs, const char *path);
void appendFile(fs::FS &fs, const char *path, const char *message);

#endif