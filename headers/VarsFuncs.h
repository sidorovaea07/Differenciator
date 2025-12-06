#ifndef HEADERS_VARSFUNCS_H
#define HEADERS_VARSFUNCS_H

#include "Trees.h"
#include "Dump.h"

int GetVariables(tree_t** t);
elem_t VarsRealloc(tree_t** t, char var);
elem_t FindVar(tree_t** t, char var);
int VarsChecker(node_t* node);

#endif