/* dokvisitor.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokvisitor.h"



/* DokVisitorFuncs:
 */
typedef struct _DokVisitorFuncs
{
  DokVisitorFunc accept;
  DokVisitorFunc enter;
  DokVisitorFunc leave;
}
  DokVisitorFuncs;



/* dok_visitor_class_new:
 */
DokVisitorClass *dok_visitor_class_new ( void )
{
  DokVisitorClass *cls;
  cls = g_new0(DokVisitorClass, 1);
  cls->table = g_hash_table_new(NULL, NULL);
  return cls;
}



/* dok_visitor_class_register_funcs:
 */
void dok_visitor_class_register_funcs ( DokVisitorClass *cls,
                                        gpointer node_type,
                                        DokVisitorFunc accept,
                                        DokVisitorFunc enter,
                                        DokVisitorFunc leave )
{
  DokVisitorFuncs *funcs;
  if (!(funcs = g_hash_table_lookup(cls->table, node_type)))
    {
      funcs = g_new0(DokVisitorFuncs, 1);
      g_hash_table_insert(cls->table, node_type, funcs);
    }
  if (accept)
    funcs->accept = accept;
  if (enter)
    funcs->enter = enter;
  if (leave)
    funcs->leave = leave;
}



/* dok_visitor_new:
 */
DokVisitor *dok_visitor_new ( DokVisitorClass *cls )
{
  DokVisitor *visitor;
  ASSERT(cls->instance_size >= sizeof(DokVisitor));
  visitor = g_malloc0(cls->instance_size);
  visitor->dok_visitor_class = cls;
  return visitor;
}



/* dok_visitor_visit:
 */
void dok_visitor_visit ( DokVisitor *visitor,
                         gpointer node )
{
  DokVisitorClass *cls = visitor->dok_visitor_class;
  gpointer node_type;
  DokVisitorFuncs *funcs;
  if (!node)
    /* [FIXME] cls->enter/leave_null() */
    return;
  ASSERT(cls->get_node_type);
  node_type = cls->get_node_type(node);
  funcs = g_hash_table_lookup(cls->table, node_type);
  /* enter */
  if (funcs && funcs->enter)
    funcs->enter(visitor, node);
  else if (cls->enter_default)
    cls->enter_default(visitor, node);
  /* accept */
  if (funcs && funcs->accept)
    funcs->accept(visitor, node);
  else if (cls->accept_default)
    cls->accept_default(visitor, node);
  /* leave */
  if (funcs && funcs->leave)
    funcs->leave(visitor, node);
  else if (cls->leave_default)
    cls->leave_default(visitor, node);
}
