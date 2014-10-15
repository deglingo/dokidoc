/* doktreetmpldumper.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktreetmpldumper.h"



/* DokTreeTmplDumper:
 */
typedef struct _DokTreeTmplDumper
{
  DokTreeVisitor visitor;
}
  DokTreeTmplDumper;



static void enter_default ( DokVisitor *visitor,
                            DokTree *tree );
static void leave_default ( DokVisitor *visitor,
                            DokTree *tree );



/* dok_tree_tmpl_dumper_get_class:
 */
static DokVisitorClass *dok_tree_tmpl_dumper_get_class ( void )
{
  static DokVisitorClass *cls = NULL;
  if (!cls)
    {
      cls = dok_tree_visitor_class_new();
      cls->instance_size = sizeof(DokTreeTmplDumper);
      cls->enter_default = (DokVisitorFunc) enter_default;
      cls->leave_default = (DokVisitorFunc) leave_default;
#define REG(up, low) dok_visitor_class_register_funcs   \
        (cls,                                           \
         GUINT_TO_POINTER(DOK_TYPE_TREE_##up),          \
         NULL,                                          \
         (DokVisitorFunc) enter_##low,                  \
         (DokVisitorFunc) leave_##low)
      /* REG(ROOT, root); */
      /* REG(DECL, decl); */
#undef REG
    }
  return cls;
}



/* dok_tree_tmpl_dumper_new:
 */
DokVisitor *dok_tree_tmpl_dumper_new ( void )
{
  DokVisitor *v = dok_visitor_new(dok_tree_tmpl_dumper_get_class());
  /* DokTreeTmplDumper *p = (DokTreeTmplDumper *) v; */
  /* p->context = g_slist_prepend(NULL, p->tree); */
  return v;
}



/* enter_default:
 */
static void enter_default ( DokVisitor *visitor,
                            DokTree *node )
{
  CL_TRACE("%s", dok_tree_to_string(node));
}



/* leave_default:
 */
static void leave_default ( DokVisitor *visitor,
                            DokTree *node )
{
  CL_TRACE("%s", dok_tree_to_string(node));
}
