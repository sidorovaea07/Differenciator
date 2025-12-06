#ifndef HEADERS_DUMP_H
#define HEADERS_DUMP_H

#include "Trees.h"
#include "FileFuncs.h"

#define CREATE_NODE(col, mode, op, pole)                                                                                                                                                \
        fprintf(fp, "\t _%p_ [shape = Mrecord; style = filled; fillcolor = pink2; color = " #col ";", node);                    \
        fprintf(fp, " label = \"{ptr = %p | type = " #op " |parent = %p | data = " mode " | left = %p | right = %p}\"]\n",      \
                            node, node->parent, ND.pole, L, R                                                                   \
);

#define CREATE_LINK(x, y) fprintf(fp, "\t_%p_->_%p_ [color = blue]\n", x, y);

#define MAKEDIR(name) do {                                                            \
                        char* iterate = (char *)calloc(STRSIZE, sizeof(char));        \
                        sprintf(iterate, "touch %s", name);                           \
                        system(iterate);                                              \
                        free(iterate);                                                \
} while (0);


int PreOrderPrint(node_t* node, FILE* fp);
int TxtGenerate(node_t* node, FILE* fp);
int PngGenerate(files_t *files);
int TreeDump(node_t *node, files_t *files);
int SaveToFile(node_t* node, files_t *files);
void CleanBuff();
int OutputName(files_t *files);
int HtmlGenerate(node_t* node, files_t *files);
int InOrderPrint(node_t* node);

int StartTex(files_t* files);
int TexDump(tree_t* t, const char* tex);
int NodeTex(node_t* node, FILE* fp);
char* TexName(files_t *files);
int EndTex(files_t* files);

#endif
