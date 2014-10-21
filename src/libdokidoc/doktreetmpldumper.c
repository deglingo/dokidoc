/* doktreetmpldumper.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktreetmpldumper.h"
#include "libdokidoc/doktemplate.h"



/* DokTreeTmplDumper:
 */
typedef struct _DokTreeTmplDumper
{
  DokTreeVisitor visitor;

  GHashTable *tmplmap;
}
  DokTreeTmplDumper;



static void enter_default ( DokVisitor *visitor,
                            DokTree *tree );
static void leave_default ( DokVisitor *visitor,
                            DokTree *tree );
static void enter_root ( DokVisitor *visitor,
                         DokTree *tree );
static void leave_root ( DokVisitor *visitor,
                         DokTree *tree );
static void enter_var ( DokVisitor *visitor,
                        DokTree *tree );
static void leave_var ( DokVisitor *visitor,
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
      REG(ROOT, root);
      REG(VAR, var);
#undef REG
    }
  return cls;
}



/* dok_tree_tmpl_dumper_new:
 */
DokVisitor *dok_tree_tmpl_dumper_new ( GHashTable *tmplmap )
{
  DokVisitor *v = dok_visitor_new(dok_tree_tmpl_dumper_get_class());
  DokTreeTmplDumper *p = (DokTreeTmplDumper *) v;
  p->tmplmap = tmplmap;
  return v;
}



/* enter_default:
 */
static void enter_default ( DokVisitor *visitor,
                            DokTree *node )
{
  CL_TRACE("[TODO] %s", dok_tree_to_string(node));
}



/* leave_default:
 */
static void leave_default ( DokVisitor *visitor,
                            DokTree *node )
{
  CL_TRACE("%s", dok_tree_to_string(node));
}



/* enter_root:
 */
static void enter_root ( DokVisitor *visitor,
                         DokTree *tree )
{
}



/* leave_root:
 */
static void leave_root ( DokVisitor *visitor,
                         DokTree *tree )
{
}



/* get_location:
 */
static DokTree *get_location ( DokTree *decl )
{
  GList *l;
  DokTree *found = NULL;
  for (l = DOK_TREE_DECL(decl)->locations; l; l = l->next)
    {
      DokTree *loc = l->data;
      if (DOK_TREE_LOC_ISDEF(loc))
        {
          found = loc;
          break;
        }
      else
        {
          if (!found)
            found = loc;
        }
    }
  ASSERT(found);
  return found;
}



/* enter_var:
 */
static void enter_var ( DokVisitor *visitor,
                        DokTree *tree )
{
  DokTree *loc = get_location(tree);
  DokTemplate *tmpl = g_hash_table_lookup(((DokTreeTmplDumper *) visitor)->tmplmap,
                                          DOK_TREE_ITEM_NAME(DOK_TREE_LOC_FILE(loc)));
  ASSERT(tmpl);
  dok_template_add_node(tmpl, "var", DOK_TREE_DECL_FQNAME(tree));
}



/* leave_var:
 */
static void leave_var ( DokVisitor *visitor,
                        DokTree *tree )
{
}
