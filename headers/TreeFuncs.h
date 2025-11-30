#ifndef HEADERS_TREEFUNCS_H
#define HEADERS_TREEFUNCS_H

#include "Trees.h"

int TreeInit(tree_t** t);
node_t* NodeInit(type_t type, elem_t data, node_t* left, node_t* right);
int InOrderPrint(node_t* node);
int NodeDestroy(node_t *node);
int TreeDestroy(tree_t** t);
elem_t TreeCalculate(node_t* node, tree_t* t);
#endif