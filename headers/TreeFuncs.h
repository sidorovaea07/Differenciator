#ifndef HEADERS_TREEFUNCS_H
#define HEADERS_TREEFUNCS_H

#include "Trees.h"
#include "Dump.h"

tree_t* TreeInit(tree_t** t);
node_t* NodeInit(type_t type, data_t* data, node_t* left, node_t* right);
int TreeProcess(tree_t** t, files_t* files);
// int NodeDestroy1(node_t** node);
node_t* NodeDestroy(node_t* node);
int TreeDestroy(tree_t** t);

#endif