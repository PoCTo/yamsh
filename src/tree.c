#include "tree.h"
#include "memory.h"
#include "string.h"

Tree* TreeInit(){
    Tree* T=NULL;
    T=myrealloc(T,sizeof(Tree));
    T->args=NULL; T->in=NULL; T->out=NULL; T->append=NULL;
    T->type=LINK_NULL;
    T->left=NULL; T->right=NULL;
    T->cmd=NULL;
    T->parent=NULL;
    return T;
}

void TreeAddRedirection(Tree *node, char *type, char *filename)
{
  if (!strcmp("<", type))
    node->in = filename;
  else if (!strcmp(">", type))
    node->out = filename;
  else if (!strcmp(">>", type))
    node->append = filename;
}

int operator_type(const char *operator)
{
  struct operator_definition {
    const char *operator;
    int type;
  } operator_definitions[] =
  {
    {"|",  LINK_PIPE},
    {";",  LINK_SEMICOLON},
    {"&",  LINK_BACKGROUND},
    {"&&", LINK_AND},
    {"||", LINK_OR}
  };
  int n_operator_definitions =
    sizeof(operator_definitions)/sizeof(struct operator_definition);

  size_t i;
  for (i=0; i<n_operator_definitions; i++)
    if (!strcmp(operator_definitions[i].operator, operator))
      return operator_definitions[i].type;

  return LINK_NULL;
}


Tree* TreeNewOperator(char *operator){
    Tree* T=TreeInit();
    T->type=operator_type(operator);
    T->cmd=operator;
    return T;
}

Tree* TreeNewSubshell(List* expression){
  Tree* node = TreeInit();
  node->type=LINK_SUBSHELL;
  node->cmd=NULL;
  return node;
}

void TreeFreeNode(Tree* node){
    
    myfree(node->append);
    ListClear(node->args);
    myfree(node->cmd);
    myfree(node->in);
    myfree(node->out);
    myfree(node);
}

void TreeFree(Tree* root){
  if (root->left != NULL)
    TreeFreeNode(root->right);

  if (root->right != NULL)
    TreeFreeNode(root->right);

  TreeFreeNode(root);
}

void TreeSwap(Tree* T1, Tree* T2){
    int type=T1->type;
    char* cmd=T1->cmd;
    char *in=T1->in,*out=T1->out,*append=T1->append;
    List* args=T1->args;
    struct Tree *left=T1->left,*right=T1->right;

    T1->type=T2->type; T1->cmd=T2->cmd;
    T1->in=T2->in; T1->out=T2->out; T1->append=T2->append;
    T1->args=T2->args; T1->left=T2->left; T1->right=T2->right;

    T2->type=type; T2->cmd=cmd;
    T2->in=in; T2->out=out; T2->append=append;
    T2->args=args; T2->left=left; T2->right=right;
}
