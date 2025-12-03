#ifndef HEADERS_CALCFUNCS_H
#define HEADERS_CALCFUNCS_H

#include "Trees.h"
#include "Dump.h"

elem_t TreeCalculate(node_t* node, tree_t** t);
int GetVariables(tree_t** t);
elem_t VarsRealloc(tree_t** t, char var);

elem_t FindVar(tree_t** t, char var);

tree_t* TreeDiff(tree_t** t);
node_t* NodeDiff(node_t* old_, char difvar);
node_t* NodeCpy(node_t* old_);
node_t* TreeOptimize1(tree_t** t, node_t* node, size_t* i);
int VarsChecker(node_t* node, tree_t** t);
node_t* TreeOptimize2(tree_t** t, node_t* node, size_t* i);

#endif