#include "TreeFuncs.h"
#include "FileWork.h"
#include "Dump.h"
#include "Needed.h"
#include <stdlib.h>

#define CREATE_NODE(col, mode, op, pole)                                                                                                                                                \
        fprintf(fp, "\t _%p_ [shape = Mrecord; style = filled; fillcolor = pink2; color = " #col ";", node);                  \
        fprintf(fp, " label = \"{parent = %p | type = " #op " |ptr = %p | data = " mode " | left = %p | right = %p}\"]\n",      \
                            node->parent, node, node->data.pole, node->left, node->right);

#define CREATE_LINK(x, y)                                                                                        \
        fprintf(fp, "\t_%p_->_%p_ [color = blue]\n", x, y);

#define MAKEDIR(name) do {                                        \
    char* iterate = (char *)calloc(STRSIZE, sizeof(char));        \
    sprintf(iterate, "touch %s", name);                           \
    system(iterate);                                              \
    free(iterate);                                                \
} while (0);

int FilesInit(files_t *files, const char* input, char* outpng, const char* htmlfile, const char* inputxt, const char* outxt)
{
    MAKEDIR(input)
    MAKEDIR(htmlfile)    
    MAKEDIR(outxt)
    files->input = input;
    files->outpng = outpng;
    files->htmlfile = htmlfile;
    files->outxt = outxt;
    files->inputxt = inputxt;
    return OK;
}

int HtmlGenerate(node_t* node, files_t *files)
{
    FILE *fp = fopen(files->htmlfile, "w");
    fprintf(fp, "<pre>\n" "<h3> TREE %p DUMP </h3>\n", node);
    PreOrderPrint(node, fp);
    fprintf(fp, "<h2> IMAGE: </h2> \n<img src = %s width = 500px> \n", files->outpng);
    fclose(fp);
    return OK;
}

int PreOrderPrint(node_t* node, FILE* fp)
{
    fprintf(fp, "(\"%g\"", node->data);
    if (node->left) {
        PreOrderPrint(node->left, fp);
    }
    else {
        fprintf(fp, "nil");
    }
    if (node->right) 
        PreOrderPrint(node->right, fp);
    else {
        fprintf(fp, "nil");
    }
    fprintf(fp, ")");
    return OK;
}

int SaveToFile(node_t* node, files_t* files)
{
    FILE* fp = fopen(files->outxt, "w");
    PreOrderPrint(node, fp);
    fclose(fp);
    return OK;
}

int TxtGenerate(node_t* node, FILE* fp)
{
    if (node->left) {
        TxtGenerate(node->left, fp);
        CREATE_LINK(node, node->left);
    }

    switch (node->type) {
        case TYPE_OP:
            CREATE_NODE(pink2, "%c", TYPE_OP, op);
            break;
        case TYPE_NONE:
            CREATE_NODE(red, "%d", TYPE_NONE, num);
            break;
        case TYPE_NUM:
            CREATE_NODE(green, "%g", TYPE_NUM, num);
            break;
        case TYPE_VAR:
            CREATE_NODE(blue, "%c", TYPE_VAR, var);
            break;
        default:
            PRINT("UNKNOWN TYPE IN TXTGENERATE()");
            break;
    }

    if (node->right) {
        TxtGenerate(node->right, fp);
        CREATE_LINK(node, node->right);
    }
    return OK;
}

int PngGenerate(files_t *files)
{
    CleanBuff();
    char str[STRSIZE] = "";
    sprintf(str, "dot %s -T png -o %s", files->input, files->outpng);
    PRS(str);
    int ret_val = system(str);
    PRD(ret_val);
    return OK;
}

int OutputName(files_t *files)
{
    char name[STRSIZE / 10] = "";
    files->outpng = (char* )calloc(STRSIZE, sizeof(char));
    printf("\nOutput file name: ");
    scanf("%9s", name);
    sprintf(files->outpng, "pictures/%s.png", name);
    PRS(files->outpng);
    return OK;
}

int TreeDump(node_t *node, files_t *files)
{
    FILE* fp = fopen(files->input, "w");
    fprintf(fp, "digraph {\n");

    TxtGenerate(node, fp);
    
    fprintf(fp, "}\n");
    fclose(fp);

    OutputName(files);    

    PngGenerate(files);
    HtmlGenerate(node, files);
    
    return OK;
}

