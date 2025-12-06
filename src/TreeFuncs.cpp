#include "TreeFuncs.h"
#include "CalcFuncs.h"
#include "FileFuncs.h"
#include "VarsFuncs.h"
#include "Debug.h"
#include "Dump.h"
#include "DSL.h"

#include <stdlib.h>
#include <math.h>

tree_t* TreeInit(tree_t** t)
{
    *t = (tree_t *) calloc(1, sizeof(tree_t));
    RT = NodeInit(TYPE_NONE, NULL, NULL, NULL);
    RT->parent = NULL;
    (*t)->size = 0;
    (*t)->nvars = 0;
    return *t;
}

node_t* NodeInit(type_t type, data_t* data, node_t* left, node_t* right)
{
    node_t* node = (node_t *) calloc(1, sizeof(node_t));
    NT = type;
    L = left;
    R = right;
    if (L)
        L->parent = node;
    if (R)
        R->parent = node;
    switch (type) {
        OP 
            ND.op = data->op; break;
        VAR 
            ND.var = data->var; break;
        NUM 
            ND.num = data->num; break;
        NONE 
            ND.num = POISON; break;
        DFLT    
    }
    return node;
}

void CleanBuff()
{
    while (getchar() != '\n');
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
                printf(GREEN "\nTHE ANSWER IS: %g\n" WHITE, TreeCalculate(RT, t));
                break;
            case 'p':
                InOrderPrint(RT);
                putchar('\n');
                break;
            case 'v':
                TreeDump(RT, files);
                break;
            case 't':
                StartTex(files);
                TexDump(*t, files->tex);
                break;
            case 'd': {
                tree_t* t_ = TreeDiff(t);
                DO(InOrderPrint(t_->root));
                DO(putchar('\n'));
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
                RT = TreeOptimize1(t, RT, &i);
                PRS("optimized 1\n");
                DO(TreeDump(RT, files));
                DO(InOrderPrint(RT));
                RT = TreeOptimize2(t, RT, &i);   
                PRS("optimized 2\n");
                DO(TreeDump(RT, files));
                break;
            }
            case 'q':
                EndTex(files);
                TreeDestroy(t);
                RET
                break;
            default:
                printf("Your symbol is: %c. Try again.\n", w);
                break;
        }
        CleanBuff();
    }
    RET    
}

node_t* NodeDestroy(node_t* node)
{
    if (!node)
        return NULL;
    if (L)
        NDL_
    if (R)
        NDR_
    free(node);
    return NULL;
}

int TreeDestroy(tree_t** t)
{
    NodeDestroy(RT);
    PRP(RT);
    free((*t)->vars);
    free(*t);
    RET
}