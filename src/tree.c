#include "tree.h"
#include "memory.h"
#include "string.h"

Tree* TreeInit(){
    Tree* T;
    T=myrealloc(T,sizeof(Tree));
    T->args=NULL; T->in=NULL; T->out=NULL; T->append=NULL;
    T->type=LINK_NULL;
    T->expr=NULL;
    T->left=NULL; T->right=NULL;
    T->cmd=NULL;
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
  node->expr = expression;
  return node;
}

void TreeFreeNode(Tree* node){
    if (node->expr != NULL){
        List* expr = node->expr;
        while (expr != NULL){
            TreeFree((Tree*)ListHead(expr));
            expr = ListPop(expr);
        }
    }
    free(node->append);
    ListClear(node->args);
    ListClear(node->expr);
    free(node->cmd);
    free(node->in);
    free(node->out);
    free(node);
}

void TreeFree(Tree* root){
  if (root->left != NULL)
    TreeFreeNode(root->right);

  if (root->right != NULL)
    TreeFreeNode(root->right);

  TreeFreeNode(root);
}


