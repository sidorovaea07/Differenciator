#include "Trees.h"
#include "TreeFuncs.h"
#include "Dump.h"
#include "FileWork.h"

int main()
{  
    tree_t* tree1 = NULL;
    readed_t read1 = {};
    files_t files1 = {};
    FilesInit(&files1, "logs/graph.txt", "", "logs/my.html", "logs/input.txt", "logs/output.txt");
    ReadFromFile(&files1, &read1, &tree1);

    printf(GREEN "THE ANSWER IS: %g\n" WHITE, TreeCalculate(RT, tree1));

    InOrderPrint(RT);
    TreeDump(RT, &files1);

    NodeDestroy(RT);
    TreeDestroy(&tree1);
    FileNamesDestroy(files1.outpng);
    // BuffDestroy(&read1);
    FINISH
    return 0;
}
       
