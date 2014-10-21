/* dokvisitor.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokvisitor.h"

#include <string.h>



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



static void lookup_funcs ( DokVisitorClass *cls,
                           gpointer node_type,
                           DokVisitorFuncs *funcs )
{
  gpointer type = node_type;
  memset(funcs, 0, sizeof(DokVisitorFuncs));
  while (type)
    {
      DokVisitorFuncs *f;
      if ((f = g_hash_table_lookup(cls->table, type)))
        {
          if ((!funcs->accept) && f->accept)
            funcs->accept = f->accept;
          if ((!funcs->enter) && f->enter)
            funcs->enter = f->enter;
          if ((!funcs->leave) && f->leave)
            funcs->leave = f->leave;
          if (funcs->accept && funcs->enter && funcs->leave)
            break;
        }
      ASSERT(cls->get_parent_type);
      type = cls->get_parent_type(type);
    }
  if (!funcs->accept)
    funcs->accept = cls->accept_default;
  if (!funcs->enter)
    funcs->enter = cls->enter_default;
  if (!funcs->leave)
    funcs->leave = cls->leave_default;
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
  DokVisitorFuncs funcs;
  if (!node)
    /* [FIXME] cls->enter/leave_null() */
    return;
  ASSERT(cls->get_node_type);
  node_type = cls->get_node_type(node);
  lookup_funcs(cls, node_type, &funcs);
  /* enter */
  if (funcs.enter)
    funcs.enter(visitor, node);
  /* accept */
  if (funcs.accept)
    funcs.accept(visitor, node);
  /* leave */
  if (funcs.leave)
    funcs.leave(visitor, node);
}
