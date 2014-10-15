/* doktreevisitor.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktreevisitor.h"



static gpointer get_node_type ( gpointer node );

static void accept_default ( DokVisitor *v,
                             DokTree *n );
static void accept_root ( DokVisitor *v,
                          DokTree *n );
static void accept_decl ( DokVisitor *v,
                          DokTree *n );



/* dok_tree_visitor_class_new:
 */
DokVisitorClass *dok_tree_visitor_class_new ( void )
{
  DokVisitorClass *cls;
  cls = dok_visitor_class_new();
  cls->get_node_type = get_node_type;
  cls->accept_default = (DokVisitorFunc) accept_default;
#define REG(tp, name) dok_visitor_class_register_funcs  \
    (cls,                                               \
     GUINT_TO_POINTER(tp),                              \
     (DokVisitorFunc) accept_##name,                    \
     NULL,                                              \
     NULL)
  REG(DOK_TYPE_TREE_ROOT, root);
  REG(DOK_TYPE_TREE_DECL, decl);
#undef REG
  return cls;
}



/* get_node_type:
 */
static gpointer get_node_type ( gpointer node )
{
  return GUINT_TO_POINTER(DOK_TYPE_INSTANCE_GET_TYPE(node));
}



/* accept_default:
 */
static void accept_default ( DokVisitor *v,
                             DokTree *n )
{
  CL_TRACE("%s", dok_tree_to_string(n));
}



/* accept_root:
 */
static void accept_root ( DokVisitor *v,
                          DokTree *n )
{
  guint i;
  for (i = 0; i < DOK_TREE_ROOT(n)->decls->len; i++)
    dok_visitor_visit(v, DOK_TREE_ROOT(n)->decls->pdata[i]);
}



/* accept_decl:
 */
static void accept_decl ( DokVisitor *v,
                          DokTree *n )
{
}
