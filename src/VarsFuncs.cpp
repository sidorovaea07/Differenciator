#include "VarsFuncs.h"
#include "Debug.h"
#include "DSL.h"

#include <stdlib.h>
#include <math.h>
#define TNV (*t)->nvars
#define TV (*t)->vars

elem_t FindVar(tree_t** t, char var)                
{
    for (size_t i = 0; i < TNV; i++) {
        PRINT("%c...\n", TV[i].name);
        if (TV[i].name == var) {
            PRD((int)TV[i].val);
            return TV[i].val;
        }
    }
    return VarsRealloc(t, var);  
}

int GetVariables(tree_t** t)
{
    InOrderPrint(RT);
    printf(GREEN "\nHOW MANY VARIABLES DO YOU WANT TO ADD? " WHITE);
    scanf("%lu", &TNV);
    PRU(TNV);
    TV = (variables_t* )calloc(TNV, sizeof(variables_t));
    for (size_t i = 0; i < TNV; i++) {
        scanf("%*c%c%lg", &TV[i].name, &TV[i].val);
        printf("added: %c\n", TV[i].name);
    }
    RET
}

int VarsChecker(node_t* node)
{
    ASSERT_N
    switch (NT) {
        NUM
            RET
        VAR
            return 1;
        OP
            return (VarsChecker(L) + VarsChecker(R));
        NONE 
            break;
        DFLT
    }
    RET
}

elem_t VarsRealloc(tree_t** t, char var)
{
    TNV++;
    TV = (variables_t* )realloc(TV, TNV*sizeof(variables_t));
    TV[TNV - 1].name = var;
    printf(RED "YOU HAVEN'T MENTIONED %c IN VARIABLES. ENTER IT'S VALUE: " WHITE, var);
    CleanBuff();
    scanf("%lg", &TV[TNV - 1].val);
    return TV[TNV - 1].val;
}