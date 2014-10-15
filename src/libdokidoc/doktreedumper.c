/* doktreedumper.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktreedumper.h"



/* DokTreeDumper:
 */
typedef struct _DokTreeDumper
{
  DokTreeVisitor visitor;

  FILE *file;
  guint depth;
}
  DokTreeDumper;



static void enter_default ( DokTreeDumper *visitor,
                            DokTree *node );
static void leave_default ( DokTreeDumper *visitor,
                            DokTree *node );



/* dok_tree_dumper_get_class:
 */
static DokVisitorClass *dok_tree_dumper_get_class ( void )
{
  static DokVisitorClass *cls = NULL;
  if (!cls)
    {
      cls = dok_tree_visitor_class_new();
      cls->instance_size = sizeof(DokTreeDumper);
      cls->enter_default = (DokVisitorFunc) enter_default;
      cls->leave_default = (DokVisitorFunc) leave_default;
    }
  return cls;
}



/* dok_tree_dumper_new:
 */
DokVisitor *dok_tree_dumper_new ( FILE *file )
{
  DokVisitor *v = dok_visitor_new(dok_tree_dumper_get_class());
  DokTreeDumper *d = (DokTreeDumper *) v;
  d->file = file;
  return v;
}



/* enter_default:
 */
static void enter_default ( DokTreeDumper *visitor,
                            DokTree *node )
{
  gint t;
  gchar *str = dok_tree_to_string(node);
  fprintf(visitor->file, " | ");
  for (t = 0; t < visitor->depth; t++)
    fprintf(visitor->file, "  ");
  fprintf(visitor->file, str);
  fprintf(visitor->file, "\n");
  g_free(str);
  visitor->depth++;
}



/* leave_default:
 */
static void leave_default ( DokTreeDumper *visitor,
                            DokTree *node )
{
  visitor->depth--;
}
