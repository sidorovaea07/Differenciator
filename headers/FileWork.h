#ifndef HEADERS_FILEWORK_H
#define HEADERS_FILEWORK_H

#include <stdio.h>
#include "Trees.h"
#include "Dump.h"


size_t FileSize(FILE *fp);
char* ReadFile(const char* myfile, size_t* ReadSize);
FILE* OpenFile(const char *myfile, const char *mode);
int FileNamesDestroy(char* name);
int BuffDestroy(readed_t* read);
node_t* GetG(char** buff);
node_t* GetE(char** buff);
node_t* GetT(char** buff);
node_t* GetP(char** buff);
node_t* GetN(char** buff);


#endif