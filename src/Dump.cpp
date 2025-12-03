#include "Dump.h"
#include "Debug.h"
#include "FileFuncs.h"

#include <stdlib.h>

#define CREATE_NODE(col, mode, op, pole)                                                                                                                                                \
        fprintf(fp, "\t _%p_ [shape = Mrecord; style = filled; fillcolor = pink2; color = " #col ";", node);                  \
        fprintf(fp, " label = \"{parent = %p | type = " #op " |ptr = %p | data = " mode " | left = %p | right = %p}\"]\n",      \
                            node->parent, node, node->data.pole, node->left, node->right);

#define CREATE_LINK(x, y)                                                                                        \
        fprintf(fp, "\t_%p_->_%p_ [color = blue]\n", x, y);

#define MAKEDIR(name) do {                                                            \
                        char* iterate = (char *)calloc(STRSIZE, sizeof(char));        \
                        sprintf(iterate, "touch %s", name);                           \
                        system(iterate);                                              \
                        free(iterate);                                                \
                    } while (0);

int FilesInit(files_t *files, const char* input, char* outpng, const char* htmlfile, const char* inputxt, const char* outxt, char* outtex)
{
    MAKEDIR(input)
    MAKEDIR(htmlfile)    
    MAKEDIR(outxt)
    files->input = input;
    files->outpng = outpng;
    files->htmlfile = htmlfile;
    files->outxt = outxt;
    files->inputxt = inputxt;
    files->tex = outtex;
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
    fprintf(fp, "(\"%p\"", &node->data);
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
            CREATE_NODE(red, "%g", TYPE_NONE, num);
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
    sprintf(files->outpng, "pictures/%s.png\n", name);
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
    free(files->outpng);
    return OK;
}

int InOrderPrint(node_t* node)
{
    putchar('(');
    if (node->left) 
        InOrderPrint(node->left);

    switch (node->type) {
        case TYPE_OP: printf(" %c ", node->data.op); break;
        case TYPE_VAR: printf(" %c ", node->data.var); break;
        case TYPE_NUM: printf(" %lg ", node->data.num); break;
        case TYPE_NONE:
        default: printf("UNKNOWN TYPE\n");    
    }    
    
    if (node->right) 
        InOrderPrint(node->right);
    putchar(')');
    return OK;
}

char* TexName(files_t *files)
{
    char name[STRSIZE / 10] = "";
    files->tex = (char* )calloc(STRSIZE, sizeof(char));
    printf("\nOutput file name: ");
    scanf("%9s", name);
    sprintf(files->tex, "logs/%s.tex", name);
    PRS(files->tex);
    return files->tex;
}

int NodeTex(node_t* node, FILE* fp)
{
    switch (node->type) {
        case TYPE_NUM:
            fprintf(fp, " %g ", node->data.num);
            break;
        case TYPE_VAR:
            fprintf(fp, " %c ", node->data.var);
            break;
        case TYPE_OP:
            switch (node->data.op) {
                case ADD:
                    NodeTex(node->left, fp);
                    fprintf(fp, "+");
                    NodeTex(node->right, fp);
                    break;
                case SUB:
                    NodeTex(node->left, fp);
                    fprintf(fp, "+");
                    NodeTex(node->right, fp);
                    break;
                case MUL:
                    fprintf(fp, "(");
                    NodeTex(node->left, fp);
                    fprintf(fp, ") \\cdot (");
                    NodeTex(node->right, fp);
                    fprintf(fp, ")");
                    break;
                case DIV: {
                    fprintf(fp, "\\frac{");
                    NodeTex(node->left, fp);
                    fprintf(fp, "}{");
                    NodeTex(node->right, fp);
                    fprintf(fp, "}");
                    break;
                }
                case POW:
                    fprintf(fp, "(");
                    NodeTex(node->left, fp);
                    fprintf(fp, ")^{");
                    NodeTex(node->right, fp);
                    fprintf(fp, "}");
                    break;
                default:
                    PRINT("DEFAULT CASE - UNKNOWN OPERATOR() REACHED\n");
                    break;
            }
        case TYPE_NONE: break;
        default:
            PRINT("DEFAULT CASE OF NODETEX() REACHED\n");
    }
    return OK;
}
//                 "\\usepackage[T2A]{fontenc}\n"   
                // "\\usepackage{listings}\n"
           
int TexDump(tree_t* t, const char* tex)
{
    FILE *fp = OpenFile(tex, "w+");
    fprintf(fp, "\\documentclass[12pt,a4paper]{article}\n"  \
                "\\usepackage[utf8]\n"                      \
                "\\usepackage[english]\n"                   \
                "\\usepackage{xcolor}\n"                    \
                "\\usepackage{geometry}\n"                  \
                "\\usepackage{caption}\n"                   \
                "\\geometry{left=2cm,right=2cm,top=2cm,bottom=2cm}\n" \
                "\\begin{document}\n"
    );
    fprintf(fp, "\\[");
    NodeTex(t->root, fp);
    fprintf(fp, "\\]\n");
    fprintf(fp, "\n\\end{document}\n");
    fclose(fp);

    char* str = (char* )calloc(STRSIZE, sizeof(char));
    sprintf(str, "pdflatex %s", tex);
    PRD(system(str));
    free(str);
    return OK;
}