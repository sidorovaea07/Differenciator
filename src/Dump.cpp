#include "Dump.h"
#include "Debug.h"
#include "FileFuncs.h"
#include "DSL.h"

#define TEX_(s1, s2, s3)    fprintf(fp, s1);           \
                            NodeTex(L, fp);             \
                            fprintf(fp, s2);           \
                            NodeTex(R, fp);             \
                            fprintf(fp, s3)            \
;

#include <stdlib.h>

int HtmlGenerate(node_t* node, files_t *files)
{
    FILE *fp = fopen(files->htmlfile, "w");
    fprintf(fp, "<pre>\n" "<h3> TREE %p DUMP </h3>\n", node);
    PreOrderPrint(node, fp);
    fprintf(fp, "<h2> IMAGE: </h2> \n<img src = %s width = 500px> \n", files->outpng);
    fclose(fp);
    RET
}

int PreOrderPrint(node_t* node, FILE* fp)
{
    fprintf(fp, "(\"%p\"", &ND);
    if (L) {
        PreOrderPrint(L, fp);
    }
    else {
        fprintf(fp, "nil");
    }
    if (R) 
        PreOrderPrint(R, fp);
    else {
        fprintf(fp, "nil");
    }
    fprintf(fp, ")");
    RET
}

int SaveToFile(node_t* node, files_t* files)
{
    FILE* fp = fopen(files->outxt, "w");
    PreOrderPrint(node, fp);
    fclose(fp);
    RET
}

int TxtGenerate(node_t* node, FILE* fp)
{
    if (!node) {
        fprintf(fp, "\t _(nil)_ [shape = Mrecord; style = filled; fillcolor = red; color = blue; label = nil]");
    }

    if (L) {
        TxtGenerate(L, fp);
        CREATE_LINK(node, L);
    }

    switch (NT) {
        OP
            CREATE_NODE(pink2, "%c", TYPE_OP, op);
            break;
        NONE
            CREATE_NODE(red, "%g", TYPE_NONE, num);
            break;
        NUM
            CREATE_NODE(green, "%g", TYPE_NUM, num);
            break;
        VAR
            CREATE_NODE(blue, "%c", TYPE_VAR, var);
            break;
        DFLT
    }

    if (R) {
        TxtGenerate(R, fp);
        CREATE_LINK(node, R);
    }
    RET
}

int PngGenerate(files_t *files)
{
    char str[STRSIZE] = "";
    sprintf(str, "dot %s -T png -o %s", files->input, files->outpng);
    PRS(str);
    int ret_val = system(str);
    PRD(ret_val);
    RET
}

int OutputName(files_t *files)
{
    char name[STRSIZE / 10] = "";
    files->outpng = (char* )calloc(STRSIZE, sizeof(char));
    printf("\nOutput file name: ");
    scanf("%9s", name);
    sprintf(files->outpng, "pictures/%s.png\n", name);
    PRS(files->outpng);
    RET
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
    RET
}

int InOrderPrint(node_t* node)
{
    putchar('(');
    ASSERT_N
    if (L) 
        InOrderPrint(L);

    switch (NT) {
        OP 
            printf(" %c ", ND.op); 
            break;
        VAR 
            printf(" %c ", ND.var); 
            break;
        NUM 
            printf(" %lg ", ND.num); 
            break;
        NONE
        DFLT
    }    
    
    if (R) 
        InOrderPrint(R);
    putchar(')');
    RET
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
    ASSERT_N
    switch (NT) {
        NUM
            fprintf(fp, " %g ", ND.num);
            break;
        VAR
            fprintf(fp, " %c ", ND.var);
            break;
        OP
            switch (ND.op) {
                case ADD:
                    TEX_("", "+", "")
                    break;
                case SUB:
                    TEX_("", "-", "")
                    break;
                case MUL:
                    TEX_("(", ") \\cdot (", ")")
                    break;
                case DIV:
                    TEX_("\\frac{", "}{", "}")
                    break;
                case POW:
                    TEX_("(", ")^{", "}")
                    break;
                DFLT
            }
        NONE break;
        DFLT
    }
    RET
}

int StartTex(files_t* files)
{
    if (files->st_tex) RET

    TexName(files);
    FILE *fp = OpenFile(files->tex, "w");
    fprintf(fp, "\\documentclass[12pt,a4paper]{article}\n\\begin{document}\n");
    fclose(fp);
    files->st_tex++;
    
    RET
}

int TexDump(tree_t* t, const char* tex)
{
    FILE *fp = OpenFile(tex, "a+");

    fprintf(fp, "\\[");
    NodeTex(t->root, fp);
    fprintf(fp, "\\]\n");
    fclose(fp);

    RET
}

int EndTex(files_t* files)
{
    if (!files->st_tex) RET

    FILE *fp = OpenFile(files->tex, "a+");
    fprintf(fp, "\n\\end{document}\n");
    fclose(fp);
    char* str = (char* )calloc(STRSIZE, sizeof(char));
    sprintf(str, "pdflatex %s", files->tex);
    PRD(system(str));
    free(str);
    free(files->tex);
    RET    
}