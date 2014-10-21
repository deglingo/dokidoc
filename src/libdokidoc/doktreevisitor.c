/* doktreevisitor.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktreevisitor.h"



static gpointer get_node_type ( gpointer node );
static gpointer get_parent_type ( gpointer type );

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
  cls->get_parent_type = get_parent_type;
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



/* get_parent_type:
 */
static gpointer get_parent_type ( gpointer type )
{
  return GUINT_TO_POINTER(dok_type_get_parent(GPOINTER_TO_UINT(type)));
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
  GList *l;
  for (l = DOK_TREE_ROOT(n)->files; l; l = l->next)
    dok_visitor_visit(v, l->data);
  dok_visitor_visit(v, DOK_TREE_ROOT(n)->nsroot);
}



/* accept_decl:
 */
static void accept_decl ( DokVisitor *v,
                          DokTree *n )
{
  GList *l;
  for (l = DOK_TREE_DECL(n)->locations; l; l = l->next)
    dok_visitor_visit(v, l->data);
  for (l = DOK_TREE_DECL(n)->members; l; l = l->next)
    dok_visitor_visit(v, l->data);
}
