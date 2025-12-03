#include "TreeFuncs.h"
#include "CalcFuncs.h"
#include "FileFuncs.h"
#include "Debug.h"

#include <stdlib.h>
#include <math.h>

#define IS_0(node, t) (!VarsChecker(node, t) && fabs(TreeCalculate(node, t)) < __DBL_MIN__)
#define CALC_L


elem_t TreeCalculate(node_t* node, tree_t** t)
{
    if (!node) return 0;
    switch (node->type) {
        case TYPE_NUM:
            return node->data.num;
        case TYPE_VAR:
            PRINT("working with %c\n", node->data.var);
            PRU((*t)->nvars);
            FindVar(t, node->data.var);
            return (*t)->vars[(*t)->nvars - 1].val;
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
                    if ((fabs(den) < __DBL_MIN__)) {
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
        case TYPE_NONE: break;
        default:
            PRINT("DEFAULT CASE OF TREECALCULATE() REACHED\n");
    }
    return POISON;
}

elem_t FindVar(tree_t** t, char var)                
{
    for (size_t i = 0; i < (*t)->nvars; i++) {
        PRINT("%c...\n", (*t)->vars[i].name);
        if ((*t)->vars[i].name == var) {
            PRD((int)(*t)->vars[i].val);
            return (*t)->vars[i].val;
        }
    }
    return VarsRealloc(t, var);
    
}

tree_t* TreeDiff(tree_t** t)
{
    tree_t* ans = {};
    TreeInit(&ans);
    printf("By which variable do you want to differentiate: ");
    scanf("%*c%c", &(*t)->difvar);
    CleanBuff();
    (*ans).root = NodeDiff((*t)->root, (*t)->difvar);
    return ans;
}

node_t* NodeCpy(node_t* old_) 
{
    node_t* new_ = (node_t* )calloc(1, sizeof(node_t));
    new_->data = old_->data;
    new_->type = old_->type;
    if (old_->left)
        new_->left = NodeCpy(old_->left);
    else
        new_->left = NULL;
    if (old_->right)
        new_->right = NodeCpy(old_->right);
    else 
        new_->right = NULL;
    return new_;
}

node_t* NodeDiff(node_t* old_, char difvar)
{
    node_t* new_ = {};
    data_t add = {ADD}, mul = {MUL}, num0 = {}, num1 = {}, div = {DIV}, pow = {POW}, num2 = {}, sub = {SUB}; num2.num = 2; num0.num = 0; num1.num = 1;
    switch (old_->type) {
        case (TYPE_NUM):
            return NodeInit(TYPE_NUM, &num0, NULL, NULL);
        case (TYPE_VAR):
            if (old_->data.var == difvar)
                return NodeInit(TYPE_NUM, &num1, NULL, NULL);
            else
                return NodeInit(TYPE_NUM, &num0, NULL, NULL);
        case (TYPE_OP):
            switch (old_->data.op) {
                case ADD:
                case SUB:
                    return NodeInit(TYPE_OP, &old_->data, NodeDiff(old_->left, difvar), NodeDiff(old_->right, difvar));         
                case MUL: 
                    return NodeInit(TYPE_OP, &add,                                                                  \
                                    NodeInit(TYPE_OP, &mul, NodeDiff(old_->left, difvar), NodeCpy(old_->right)),    \
                                    NodeInit(TYPE_OP, &mul, NodeCpy(old_->left), NodeDiff(old_->right, difvar)));         
                case DIV:
                    return NodeInit(TYPE_OP, &div,           \
                                    NodeInit(TYPE_OP, &sub,  \
                                            NodeInit(TYPE_OP, &mul, NodeDiff(old_->left, difvar), NodeCpy(old_->right)),  \
                                            NodeInit(TYPE_OP, &mul, NodeCpy(old_->left), NodeDiff(old_->right, difvar))), \
                                    NodeInit(TYPE_OP, &pow,                                                             \
                                            NodeCpy(old_->right),                                                       \
                                            NodeInit(TYPE_NUM, &num2, NULL, NULL)));
                case POW:
                default: 
                    break;
            }
        case TYPE_NONE:
        default:
            break;        
    }

    return new_;
    
}

int GetVariables(tree_t** t)
{
    InOrderPrint((*t)->root);
    printf(GREEN "\nHOW MANY VARIABLES DO YOU WANT TO ADD? " WHITE);
    scanf("%lu", &(*t)->nvars);
    PRU((*t)->nvars);
    (*t)->vars = (variables_t* )calloc((*t)->nvars, sizeof(variables_t));
    for (size_t i = 0; i < (*t)->nvars; i++) {
        scanf("%*c%c%lg", &(*t)->vars[i].name, &(*t)->vars[i].val);
        printf("added: %c\n", (*t)->vars[i].name);
    }
    return OK;
}

elem_t VarsRealloc(tree_t** t, char var)
{
    (*t)->nvars++;
    (*t)->vars = (variables_t* )realloc((*t)->vars, (*t)->nvars*sizeof(variables_t));
    (*t)->vars[(*t)->nvars - 1].name = var;
    printf(RED "YOU HAVEN'T MENTIONED %c IN VARIABLES. ENTER IT'S VALUE: " WHITE, var);
    CleanBuff();
    scanf("%lg", &(*t)->vars[(*t)->nvars - 1].val);
    return (*t)->vars[(*t)->nvars - 1].val;
}

node_t* TreeOptimize1(tree_t** t, node_t* node, size_t* i)
{
    data_t num0 = {}; num0.num = 0;
    if (IS_0(node, t)) {
        i++;
        if (node->left) NodeDestroy(node->left);
        if (node->right) NodeDestroy(node->right);
        return NodeInit(TYPE_NUM, &num0, NULL, NULL);
    }
    else {
        if (node->left) {
            node->left = TreeOptimize1(t, node->left, i);
        }
        if (node->right) {
            node->right = TreeOptimize1(t, node->right, i);
        }
    }
    return node;
}

int VarsChecker(node_t* node, tree_t** t)
{
    if (!node) return 0;
    switch (node->type) {
        case TYPE_NUM:
            return 0;
        case TYPE_VAR:
            return 1;
        case TYPE_OP:
            return (VarsChecker(node->left, t) + VarsChecker(node->right, t));
        case TYPE_NONE: break;
        default:
            PRINT("DEFAULT CASE OF TREECALCULATE() REACHED\n");
    }
    return 0;
}

node_t* TreeOptimize2(tree_t** t, node_t* node, size_t* i)
{
    if (node->type == TYPE_OP) {
        switch (node->data.op) {
            case ADD:
                if (IS_0(node->left, t)) {
                    NodeDestroy(node->left);
                }
                if (IS_0(node->right, t)) {
                    NodeDestroy(node->right);
                }
                break;
            case MUL:
                if (IS_0(node->left, t) || IS_0(node->right, t)) {
                    NodeDestroy(node->left);
                    NodeDestroy(node->right);
                    node->type = TYPE_NUM;
                    node->data.num = 0;
                }
                break;
            case POW:
                if (IS_0(node->left, t)) {
                    NodeDestroy(node->left);
                    NodeDestroy(node->right);
                    node->type = TYPE_NUM;
                    node->data.num = 0;
                }
                else if (IS_0(node->right, t)) {
                    NodeDestroy(node->right);
                    node->type = node->left->type;
                    node->data = node->left->data;
                    NodeDestroy(node->left);
                }
                break;
            case SUB:
                if (IS_0(node->right, t)) {
                    NodeDestroy(node->right);
                    node->type = node->left->type;
                    node->data = node->left->data;
                    NodeDestroy(node->left);
                }
                break;
            case DIV:
                if (IS_0(node->left, t)) {
                    NodeDestroy(node->left);
                    NodeDestroy(node->right);
                    node->type = TYPE_NUM;
                    node->data.num = 0;
                }
                break;
            default:
                PRINT("DEFAULT CASE OF TREEOPTIMIZE2() REACHED\n");       
        }
    }
    else {
        node->left = TreeOptimize2(t, node->left, i);
        node->right = TreeOptimize2(t, node->right, i);
    }
    return node;
}


