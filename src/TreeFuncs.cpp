#include "TreeFuncs.h"
#include "Dump.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>


int TreeDestroy(tree_t** t)
{
    free(*t);
    return OK;
}

int TreeInit(tree_t** t)
{
    *t = (tree_t *) calloc(1, sizeof(tree_t));    
    (*t)->root = (node_t *) calloc(1, sizeof(node_t));
    PRP((*t)->root);
    (*t)->root->data.num = POISON;
    (*t)->root->type = TYPE_NONE;
    (*t)->root->left = NULL;
    (*t)->root->right = NULL;
    (*t)->root->parent = NULL;
    (*t)->size = 0;    
    return OK;
}

node_t* NodeInit(type_t type, elem_t data, node_t* left, node_t* right)
{
    node_t* node = (node_t *) calloc(1, sizeof(node_t));
    node->type = type;
    node->left = left;
    node->right = right;
    if (node->left)
        node->left->parent = node;
    if (node->right)
        node->right->parent = node;
    switch (type) {
        case TYPE_OP: node->data.op = (operator_t)data; break;
        case TYPE_VAR: node->data.var = (int)data; break;
        case TYPE_NUM: node->data.num = (elem_t)data; break;
        default: printf("UNKNOWN TYPE\n");    
    }
    return node;
}

int InOrderPrint(node_t* node)
{
    putchar('(');
    if (node->left) 
        InOrderPrint(node->left);

    switch (node->type) {
        case TYPE_OP: printf(" %c ", node->data.op); break;
        case TYPE_VAR: printf(" %d ", node->data.var); break;
        case TYPE_NUM: printf(" %g ", node->data.num); break;
        default: printf("UNKNOWN TYPE\n");    
    }    
    
    if (node->right) 
        InOrderPrint(node->right);
    putchar(')');
    return OK;
}

elem_t TreeCalculate(node_t* node, tree_t* t)
{
    switch (node->type) {
        case TYPE_NUM:
            return node->data.num;
        case TYPE_VAR:
            while ((*(t->vars)).name != node->data.var)
                (t->vars)++;
            return (*(t->vars)).val;
        case TYPE_OP:
            switch (node->data.op) {
                case ADD:
                    return TreeCalculate(node->left, t) + TreeCalculate(node->right, t);
                case SUB:
                    return TreeCalculate(node->left, t) - TreeCalculate(node->right, t);
                case MUL:
                    return TreeCalculate(node->left, t) * TreeCalculate(node->right, t);
                case DIV: {
                    elem_t den = TreeCalculate(node->right, t);
                    if (!den) {
                        printf("We can't div on null\n");
                        return 0;
                    }
                    else 
                        return TreeCalculate(node->left, t) / TreeCalculate(node->right, t);
                }
                case POW:
                    return pow(TreeCalculate(node->left, t), TreeCalculate(node->right, t));
                default:
                    PRINT("DEFAULT CASE OF TREECALCULATE() REACHED\n");
                    break;
            }        
    }
}

int NodeDestroy(node_t* node)
{
    if (node->left) {
        NodeDestroy(node->left);
    }
    if (node->right) {
        NodeDestroy(node->right);
    }
    free(node);
    return OK;
}

int CleanBuff()
{
    while (getchar() != '\n');
    return OK;
}