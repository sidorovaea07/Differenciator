#include "Debug.h"
#include "Trees.h"
#include "TreeFuncs.h"
#include "FileFuncs.h"

int main()
{  
    tree_t* t0 = NULL; readed_t read1 = {}; files_t files1 = {};
    FilesInit(&files1, "logs/graph.txt", "", "logs/my.html", "logs/input.txt", "logs/output.txt", "");
    ReadFromFile(&files1, &read1, &t0);
    TreeProcess(&t0, &files1);
    FINISH
    return 0;
}