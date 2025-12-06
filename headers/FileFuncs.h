#ifndef HEADERS_FILEFUNCS_H
#define HEADERS_FILEFUNCS_H

#include <stdio.h>
#include "Trees.h"

typedef struct files_t
{
    const char* input;
    char* outpng;
    const char* htmlfile;
    const char* outxt;
    const char* inputxt;
    char* tex;
    size_t st_tex;
} files_t;

typedef struct readed_t
{
    size_t size;
    char* buff;
    size_t poz;
} readed_t;

size_t FileSize(FILE *fp);
char* ReadFile(const char* file, size_t* readsize, char** buff);
tree_t* ReadFromFile(files_t* files, readed_t* read, tree_t** t);
FILE* OpenFile(const char *myfile, const char *mode);
int FileNamesDestroy(char* name);
int BuffDestroy(readed_t* read);
node_t* GetG(char** buff);
node_t* GetE(char** buff);
node_t* GetT(char** buff);
node_t* GetP(char** buff);
node_t* GetN(char** buff);
node_t* GetV(char** buff);
int FilesInit(files_t *files);

#endif