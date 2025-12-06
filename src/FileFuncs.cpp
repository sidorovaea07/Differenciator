#include "TreeFuncs.h"
#include "FileFuncs.h"
#include "Debug.h"
#include "Dump.h"
#include "DSL.h"
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

size_t FileSize(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    size_t size = (size_t)ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return size;
}

FILE* OpenFile(const char *file, const char *mode)
{
    FILE *fp = fopen(file, mode);
    if (!fp) {
        printf(RED "ERROR WHILE OPENING THE FILE: %s.\n" WHITE, file);
        return NULL;
    }
    return fp;

}

char* ReadFile(const char* file, size_t* readsize, char** buff)
{
    FILE *fp = OpenFile(file, "r");
    size_t filesize = FileSize(fp);
    PRU(filesize);
    *buff = (char *) calloc(filesize + 3, sizeof (char));
    *readsize = fread(*buff, sizeof (char), filesize, fp);
    (*buff)[*readsize] = '\n';
    (*buff)[*readsize + 1] = '\0';
    fclose(fp);
    return *buff;
}

node_t* GetV(char** buff)
{
    data_t d = {}; d.var = '\0';
    node_t* new_node = NodeInit(TYPE_VAR, &d, NULL, NULL);
    PRINT("in GetV(): ");
    PRP(new_node);
    PRS(*buff);
    if ('a' <= **buff && **buff <= 'z') {
        new_node->data.var = **buff;
        (*buff)++;
    }
    PRS(*buff);
    return new_node;
}

node_t* GetN(char** buff)
{
    data_t d = {}; d.num = 0;
    node_t* new_node = NodeInit(TYPE_NUM, &d, NULL, NULL);
    PRINT("in GetN(): ");
    PRP(new_node);
    PRS(*buff);
    while ('0' <= **buff && **buff <= '9') {
        new_node->data.num = new_node->data.num * 10 + **buff - '0';
        (*buff)++;
    }
    PRS(*buff);
    return new_node;
}

node_t* GetP(char** buff)
{
    PRINT("in GetP(): ");
    PRS(*buff);
    if (**buff == '(') {
        (*buff)++;
        node_t* new_node = GetE(buff);
        (*buff)++;
        return new_node;
    }
    else if (isdigit(**buff))
        return GetN(buff);
    else
        return GetV(buff);
}

node_t* GetT(char** buff)
{
    PRINT("in GetT(): ");
    PRS(*buff);
    node_t* new_node = GetP(buff);
    while (**buff == '*' || **buff == '/') {
        int op = **buff;
        (*buff)++;
        node_t* new_node_ = GetP(buff);
        data_t d = {}; d.op = (operator_t)op;
        new_node = NodeInit(TYPE_OP, &d, new_node, new_node_);
    }
    return new_node;   
}

node_t* GetE(char** buff)
{
    PRINT("in GetE(): ");
    PRS(*buff);
    node_t* new_node = GetT(buff);
    while (**buff == '+' || **buff == '-') {
        int op = **buff;
        (*buff)++;
        node_t* new_node_ = GetT(buff);
        data_t d = {}; d.op = (operator_t)op;
        new_node = NodeInit(TYPE_OP, &d, new_node, new_node_);
    }
    return new_node;
}

node_t* GetG(char** buff)
{
    PRINT("in GetG(): ");
    PRS(*buff);
    node_t* new_node = GetE(buff);
    PRINT("in GetG() after GetE(): %s\n", *buff);
    if (**buff != '$') {
        PRINT("*buff != $ in GetG(): %s\n", *buff);
        return NULL;
    }
    (*buff)++;
    return new_node;
}

tree_t* ReadFromFile(files_t* files, readed_t* read, tree_t** t)
{
    read->size = 0;
    char* iterate = ReadFile(files->inputxt, &read->size, &read->buff);
    read->buff = iterate;
    PRS(read->buff);
    *t = TreeInit(t);
    (*t)->root = GetG(&read->buff);
    PRP((*t)->root);
    free(iterate);
    PRINT("\nREADED\n");
    return *t;
}

int FilesInit(files_t *files)
{
    MAKEDIR(files->input)
    MAKEDIR(files->htmlfile)    
    MAKEDIR(files->outxt)
    RET
}

int FileNamesDestroy(char* name)
{
    free(name);
    return 0;
}

int BuffDestroy(readed_t* read)
{
    free(read->buff);
    return 0;
}