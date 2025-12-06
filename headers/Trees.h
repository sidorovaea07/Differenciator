#ifndef HEADER_TREES_H
#define HEADER_TREES_H

#include <stdio.h>

#define OK 0
#define STRSIZE 100
const double POISON = 0xdead;
typedef double elem_t;

typedef struct variables_t {
    char name;
    elem_t val;
} variables_t;

enum operator_t {
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    POW = '^',
};

enum type_t {
    TYPE_OP,
    TYPE_VAR,
    TYPE_NUM,
    TYPE_NONE,
};

typedef union data_t {
    operator_t op;
    char var;
    elem_t num;
} data_t;

typedef struct node_t {
    type_t type;
    data_t data;
    node_t* left;
    node_t* right;
    node_t* parent;
} node_t;

typedef struct tree_t {
    node_t* root;
    size_t size;
    variables_t* vars;
    size_t nvars;
    char difvar;
} tree_t;

#endif
