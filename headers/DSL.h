#ifndef HEADERS_DSL
#define HEADERS_DSL

#define OK 0
#define RT (*t)->root
#define L node->left
#define R node->right
#define ND node->data
#define NT node->type

#define NDL_ {PRP(L); L = NodeDestroy(L); PRP(L);} 
#define NDR_ {PRP(R); R = NodeDestroy(R); PRP(R);}
#define NDL NodeDestroy(L)
#define NDR NodeDestroy(R)

#define IS_0(node, t) (!VarsChecker(node) && fabs(TreeCalculate(node, t)) < __DBL_MIN__)
#define IS_opt(opt_) (!(isnan(opt_)) && opt_<__DBL_MIN__)
#define NUM case TYPE_NUM:
#define VAR case TYPE_VAR:
#define OP case TYPE_OP:
#define NONE case TYPE_NONE:
#define INIT_NUM(x) NodeInit(TYPE_NUM, &(x), NULL, NULL);
#define DFLT default: printf("DEFAULT CASE IN %s:%d\n", __FILE__, __LINE__); break;
#define RET return OK;
#define ASSERT_N if (!node) return NULL;

#endif