#ifndef HEADERS_DUMP_H
#define HEADERS_DUMP_H

#include "Trees.h"

typedef struct files_t
{
    const char* input;
    char* outpng;
    const char* htmlfile;
    const char* outxt;
    const char* inputxt;
    char* tex;
} files_t;

typedef struct readed_t
{
    size_t size;
    char* buff;
    size_t poz;
} readed_t;

int PreOrderPrint(node_t* node, FILE* fp);
int TxtGenerate(node_t* node, FILE* fp);
int PngGenerate(files_t *files);
int TreeDump(node_t *node, files_t *files);
int SaveToFile(node_t* node, files_t *files);
int CleanBuff();
int OutputName(files_t *files);
int HtmlGenerate(node_t* node, files_t *files);
int FilesInit(files_t *files, const char* input, char* outpng, const char* htmlfile, const char* inputxt, const char* outxt, char* outtex);
int InOrderPrint(node_t* node);
int TexDump(tree_t* t, const char* tex);
int NodeTex(node_t* node, FILE* fp);
char* TexName(files_t *files);

#endif
