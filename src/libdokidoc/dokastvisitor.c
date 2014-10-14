/* dokastvisitor.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokastvisitor.h"



static gpointer get_node_type ( gpointer node );

static void accept_list ( DokVisitor *v,
                          DokAST *n );
static void accept_namespace ( DokVisitor *v,
                               DokAST *n );
static void accept_var_decl ( DokVisitor *v,
                              DokAST *n );
static void accept_pointer ( DokVisitor *v,
                             DokAST *n );



/* dok_ast_visitor_class_new:
 */
DokVisitorClass *dok_ast_visitor_class_new ( void )
{
  DokVisitorClass *cls;
  cls = dok_visitor_class_new();
  cls->get_node_type = get_node_type;
#define REG(tp, name) dok_visitor_class_register_funcs  \
    (cls,                                               \
     GUINT_TO_POINTER(tp),                              \
     (DokVisitorFunc) accept_##name,                    \
     NULL,                                              \
     NULL)
  REG(DOK_AST_LIST, list);
  REG(DOK_AST_NAMESPACE, namespace);
  REG(DOK_AST_VAR_DECL, var_decl);
  REG(DOK_AST_POINTER, pointer);
#undef REG
  return cls;
}



/* get_node_type:
 */
static gpointer get_node_type ( gpointer node )
{
  return GUINT_TO_POINTER(((DokAST *) node)->type);
}



/* accept_list:
 */
static void accept_list ( DokVisitor *v,
                          DokAST *n )
{
  DokAST *l;
  /* [FIXME] accept every list node ? */
  for (l = n; l; l = DOK_AST_LIST_NEXT(l))
    dok_visitor_visit(v, DOK_AST_LIST_ITEM(l));
}



/* accept_namespace:
 */
static void accept_namespace ( DokVisitor *v,
                               DokAST *n )
{
  dok_visitor_visit(v, DOK_AST_NAMESPACE_DECLS(n));
}



/* accept_var_decl:
 */
static void accept_var_decl ( DokVisitor *v,
                              DokAST *n )
{
  dok_visitor_visit(v, DOK_AST_DECL_TYPE(n));
}



/* accept_pointer:
 */
static void accept_pointer ( DokVisitor *v,
                             DokAST *n )
{
  dok_visitor_visit(v, DOK_AST_POINTER_TARGET(n));
}
