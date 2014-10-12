/* dokastdumper.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokastdumper.h"



/* DokASTDumper:
 */
typedef struct _DokASTDumper
{
  DokASTVisitor visitor;

  FILE *file;
  guint depth;
}
  DokASTDumper;



static void enter_default ( DokASTDumper *visitor,
                            DokAST *node );
static void leave_default ( DokASTDumper *visitor,
                            DokAST *node );



/* dok_ast_dumper_get_class:
 */
static DokVisitorClass *dok_ast_dumper_get_class ( void )
{
  static DokVisitorClass *cls = NULL;
  if (!cls)
    {
      cls = dok_ast_visitor_class_new();
      cls->instance_size = sizeof(DokASTDumper);
      cls->enter_default = (DokVisitorFunc) enter_default;
      cls->leave_default = (DokVisitorFunc) leave_default;
    }
  return cls;
}



/* dok_ast_dumper_new:
 */
DokVisitor *dok_ast_dumper_new ( FILE *file )
{
  DokVisitor *v = dok_visitor_new(dok_ast_dumper_get_class());
  DokASTDumper *d = (DokASTDumper *) v;
  d->file = file;
  return v;
}



/* enter_default:
 */
static void enter_default ( DokASTDumper *visitor,
                            DokAST *node )
{
  gint t;
  gchar *str = dok_ast_to_string(node);
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
static void leave_default ( DokASTDumper *visitor,
                            DokAST *node )
{
  visitor->depth--;
}
