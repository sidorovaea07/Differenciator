#include "TreeFuncs.h"
#include "CalcFuncs.h"
#include "FileFuncs.h"
#include "Debug.h"
#include "DSL.h"
#include "VarsFuncs.h"

#include <stdlib.h>
#include <math.h>
#define NCPY(x) x->left->parent = x->right->parent = x->parent; \
                x->parent->left = x->left;                      \
                x->parent->right = x->right;                    \
                x->parent->type = x->type;                      \
                x->parent->data = x->data;

elem_t TreeCalculate(node_t* node, tree_t** t)
{
    ASSERT_N
    switch (NT) {
        NUM
            return ND.num;
        VAR
            PRINT("working with %c\n", ND.var);
            PRU((*t)->nvars);
            FindVar(t, ND.var);
            return (*t)->vars[(*t)->nvars - 1].val;
        OP
            switch (ND.op) {
                case ADD:
                    return TreeCalculate(L, t) + TreeCalculate(R, t);
                case SUB:
                    return TreeCalculate(L, t) - TreeCalculate(R, t);
                case MUL:
                    return TreeCalculate(L, t) * TreeCalculate(R, t);
                case DIV: {
                    elem_t den = TreeCalculate(R, t);
                    if ((fabs(den) < __DBL_MIN__)) {
                        printf("We can't div on null\n");
                        RET
                    }
                    else 
                        return TreeCalculate(L, t) / TreeCalculate(R, t);
                }
                case POW:
                    return pow(TreeCalculate(L, t), TreeCalculate(R, t));
                DFLT
            }
        NONE
        DFLT
    }
    return POISON;
}

tree_t* TreeDiff(tree_t** t)
{
    tree_t* ans = {};
    TreeInit(&ans);
    printf("By which variable do you want to differentiate: ");
    scanf("%*c%c", &(*t)->difvar);
    CleanBuff();
    (*ans).root = NodeDiff(RT, (*t)->difvar);
    PRP((*ans).root);
    return ans;
}

node_t* NodeCpy(node_t* old_) 
{
    node_t* node = (node_t* )calloc(1, sizeof(node_t));
    ND = old_->data;
    NT = old_->type;
    if (old_->left)
        L = NodeCpy(old_->left);
    else
        L = NULL;
    if (old_->right)
        R = NodeCpy(old_->right);
    else 
        R = NULL;
    return node;
}

node_t* NodeDiff(node_t* old_, char difvar)
{
    node_t* node = {};
    data_t add = {ADD}, mul = {MUL}, num0 = {}, num1 = {}, div = {DIV}, pow = {POW}, num2 = {}, sub = {SUB}; num2.num = 2; num0.num = 0; num1.num = 1;
    if (!old_)
        return NULL;
    switch (old_->type) {
        NUM
            return INIT_NUM(num0)
        VAR
            if (old_->data.var == difvar)
                return INIT_NUM(num1)
            else
                return INIT_NUM(num0)
        OP
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
                    return NULL;
                DFLT
            }
        NONE
        DFLT      
    }
    return node;
}

node_t* TreeOptimize1(tree_t** t, node_t* node, size_t* i)
{
    ASSERT_N
    data_t num = {}; num.num = NAN;

    if (node->type == TYPE_OP && !VarsChecker(node)) {
        num.num = TreeCalculate(node, t);
        (*i)++;
        return NodeInit(TYPE_NUM, &num, NDL, NDR);
    }

    return NodeInit(node->type, &node->data, TreeOptimize1(t, L, i), TreeOptimize1(t, R, i));
}

elem_t Optimizer(node_t* node, tree_t** t)
{
    ASSERT_N
    switch (NT) {
        NUM
            return ND.num;
        VAR
            return NAN;
        OP
            switch (ND.op) {
                case ADD:
                    return Optimizer(L, t) + Optimizer(R, t);
                case SUB:
                    return Optimizer(L, t) - Optimizer(R, t);
                case MUL:
                    if ((L->type == TYPE_VAR && IS_0(R, t)) || (R->type == TYPE_VAR && IS_0(L, t)))
                        return 0;
                    return Optimizer(L, t) * Optimizer(R, t);
                case DIV: {
                    elem_t den = Optimizer(R, t);
                    elem_t numer = Optimizer(L, t);
                    if ((fabs(den) < __DBL_MIN__)) {
                        printf("We can't div on null\n");
                        RET
                    }
                    if ((fabs(numer) < __DBL_MIN__)) {
                        RET
                    }
                    return numer / den;
                }
                case POW:
                    return pow(Optimizer(L, t), Optimizer(R, t));
                DFLT
            }
        NONE
        DFLT
    }
    return NAN;
}

node_t* TreeOptimize2(tree_t** t, node_t* node, size_t* i)
{
    data_t num0 = {}; num0.num = 0;
    ASSERT_N
    if (NT == TYPE_OP) {
        elem_t opt_l = Optimizer(L, t);
        elem_t opt_r = Optimizer(R, t);
        if (!IS_opt(opt_l) && !IS_opt(opt_r)) {
            if (L)
                L = TreeOptimize2(t, L, i);
            if (R)
                R = TreeOptimize2(t, R, i);
        }
        switch (ND.op) {
            case ADD:
                if (IS_opt(opt_l)) {
                    (*i)++;
                    L = NDL;
                    NCPY(R)
                    break;
                }
                if (IS_opt(opt_r)) {
                    (*i)++;
                    R = NDR;
                    NCPY(L)
                }
                break;
            case SUB:
                if (IS_opt(opt_r)) {
                    PRINT("in sub:\n"); PRP(node->right);
                    (*i)++;
                    R = NDR;
                    NCPY(L)
                }
                break;
            case MUL:
                if (IS_opt(opt_l) || IS_opt(opt_r)) {
                    (*i)++;
                    L = NDL;
                    R = NDR;
                    node = NodeDestroy(node);
                    return INIT_NUM(num0);
                }
                break;
            case POW:
                if (IS_opt(opt_l)) {
                    (*i)++;
                    node = NodeDestroy(node);
                    return INIT_NUM(num0);
                }
                if (IS_opt(opt_r)) {
                    (*i)++;
                    R = NDR;
                    L->left->parent = L->right->parent = L->parent;
                    L->parent->left = L->left;
                    L->parent->right = L->right;
                    L->parent->type = L->type;
                    L->parent->data = L->data;
                }
                break;
            case DIV:
                if (IS_opt(opt_r)) {
                    (*i)++;
                    node = NodeDestroy(node);
                    return INIT_NUM(num0);
                }
                break;
            DFLT       
        }
    }
    
    return node;
}


