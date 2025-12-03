#include "TreeFuncs.h"
#include "CalcFuncs.h"
#include "FileFuncs.h"
#include "Debug.h"
#include "Dump.h"

#include <stdlib.h>
#include <math.h>

tree_t* TreeInit(tree_t** t)
{
    *t = (tree_t *) calloc(1, sizeof(tree_t));
    (*t)->root = NodeInit(TYPE_NONE, NULL, NULL, NULL);
    (*t)->root->parent = NULL;
    (*t)->size = 0;
    (*t)->nvars = 0;
    return *t;
}

node_t* NodeInit(type_t type, data_t* data, node_t* left, node_t* right)
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
        case TYPE_OP: node->data.op = data->op; break;
        case TYPE_VAR: node->data.var = data->var; break;
        case TYPE_NUM: node->data.num = data->num; break;
        case TYPE_NONE: node->data.num = POISON; break;
        default: printf("UNKNOWN TYPE\n");    
    }
    return node;
}

int CleanBuff()
{
    while (getchar() != '\n');
    return OK;
}

int TreeProcess(tree_t** t, files_t* files)
{
    char w = '\0';
    printf(GREEN "What do you want to do?\n\"c\" - to calculate\n\"d\" - to differentiate\n\"p\" - to print\n\"v\" - to dump\n\"t\" - to view pdf\n\"q\" - to quit\n" WHITE);
    while (scanf("%c", &w)) {
        printf(GREEN "OK\n" WHITE);
        switch (w) {
            case 'c':
                GetVariables(t);    
                printf(GREEN "\nTHE ANSWER IS: %g\n" WHITE, TreeCalculate((*t)->root, t));
                break;
            case 'p':
                InOrderPrint((*t)->root);
                putchar('\n');
                break;
            case 'v':
                TreeDump((*t)->root, files);
                break;
            case 't':
                TexDump(*t, TexName(files));
                break;
            case 'd': {
                tree_t* t_ = TreeDiff(t);
                t_->nvars = (*t)->nvars;
                t_->size = (*t)->size;
                t_->vars = (variables_t* )calloc(t_->nvars, sizeof(variables_t));
                for (size_t i = 0; i < t_->nvars; i++) {
                    t_->vars[i].name = (*t)->vars[i].name;
                    t_->vars[i].val = (*t)->vars[i].val;
                }
                TreeDestroy(t);
                return TreeProcess(&t_, files);
                break;
            }
            case 'o': {
                size_t i = 0;
                do {
                    (*t)->root = TreeOptimize1(t, (*t)->root, &i);
                } while (i);
                (*t)->root = TreeOptimize2(t, (*t)->root, &i);
                break;
            }
            case 'q':
                TreeDestroy(t);
                return OK;
                break;
            default:
                printf("Your symbol is: %c. Try again.\n", w);
                break;
        }
        CleanBuff();
    }
    return OK;    
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
    node = NULL;
    return OK;
}

int TreeDestroy(tree_t** t)
{
    NodeDestroy((*t)->root);
    free((*t)->vars);
    free(*t);
    return OK;
}