#include "Debug.h"
#include "DSL.h"
#include "Trees.h"
#include "TreeFuncs.h"
#include "FileFuncs.h"

int main()
{  
    tree_t* t0 = NULL; readed_t read1 = {}; 
    files_t files1 = {"logs/graph.txt", "", "logs/my.html", "logs/output.txt", "logs/input.txt", "", 0};
    FilesInit(&files1);
    ReadFromFile(&files1, &read1, &t0);
    TreeProcess(&t0, &files1);
    FINISH
    RET
}

// ощущение, что не free() корень, как пофиксить
// как установить пакеты в латех
// как создавать пдф не в корневой папке