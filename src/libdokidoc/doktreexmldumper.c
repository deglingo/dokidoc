/* doktreexmldumper.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktreexmldumper.h"



/* DokTreeXmlDumper:
 */
typedef struct _DokTreeXmlDumper
{
  DokTreeVisitor visitor;

  xmlNodePtr root;
  GSList *context;
}
  DokTreeXmlDumper;



static void enter_default ( DokVisitor *visitor,
                            gpointer node );
static void enter_root ( DokVisitor *visitor,
                         DokTree *tree );
static void leave_root ( DokVisitor *visitor,
                         DokTree *tree );
static void enter_decl ( DokVisitor *visitor,
                         DokTree *tree );
static void leave_decl ( DokVisitor *visitor,
                         DokTree *tree );



/* dok_tree_xml_dumper_get_class:
 */
static DokVisitorClass *dok_tree_xml_dumper_get_class ( void )
{
  static DokVisitorClass *cls = NULL;
  if (!cls)
    {
      cls = dok_tree_visitor_class_new();
      cls->instance_size = sizeof(DokTreeXmlDumper);
      cls->enter_default = enter_default;
#define REG(up, low) dok_visitor_class_register_funcs   \
        (cls,                                           \
         GUINT_TO_POINTER(DOK_TYPE_TREE_##up),          \
         NULL,                                          \
         (DokVisitorFunc) enter_##low,                  \
         (DokVisitorFunc) leave_##low)
      REG(ROOT, root);
      REG(DECL, decl);
#undef REG
    }
  return cls;
}



/* dok_tree_xml_dumper_new:
 */
DokVisitor *dok_tree_xml_dumper_new ( void )
{
  DokVisitor *v = dok_visitor_new(dok_tree_xml_dumper_get_class());
  /* DokTreeXmlDumper *p = (DokTreeXmlDumper *) v; */
  /* p->context = g_slist_prepend(NULL, p->tree); */
  return v;
}



/* dok_tree_xml_dumper_get_root:
 */
xmlNodePtr dok_tree_xml_dumper_get_root ( DokVisitor *visitor )
{
  return ((DokTreeXmlDumper *) visitor)->root;
}



#define proc ((DokTreeXmlDumper *) visitor)
#define ctxt (proc->context ? ((xmlNodePtr) (proc->context->data)) : NULL)
#define push(n) (proc->context = g_slist_prepend(proc->context, (n)))
#define pop() (proc->context = g_slist_delete_link(proc->context, proc->context))



/* enter_default:
 */
static void enter_default ( DokVisitor *visitor,
                            gpointer node )
{
  CL_DEBUG("[TODO] enter(%s)", dok_tree_to_string(node));
}



/* enter_root:
 */
static void enter_root ( DokVisitor *visitor,
                         DokTree *tree )
{
  ASSERT(!proc->root);
  CL_TRACE("%p", tree);
  proc->root = xmlNewNode(NULL, BAD_CAST "root");
  push(proc->root);
}



/* leave_root:
 */
static void leave_root ( DokVisitor *visitor,
                         DokTree *tree )
{
  pop();
}



/* enter_decl:
 */
static void enter_decl ( DokVisitor *visitor,
                         DokTree *tree )
{
  gchar *type_name, *id, *context;
  push(xmlNewChild(ctxt, NULL, BAD_CAST "decl", NULL));
  type_name = g_strdup_printf("%d", DOK_TREE_DECL(tree)->decl_type); /* [fixme] */
  id = g_strdup_printf("%d", DOK_TREE_DECL(tree)->id);
  context = g_strdup_printf("%d", DOK_TREE_DECL(tree)->context ? DOK_TREE_DECL(DOK_TREE_DECL(tree)->context)->id : 0);
  xmlNewProp(ctxt, BAD_CAST "id", BAD_CAST id);
  xmlNewProp(ctxt, BAD_CAST "context", BAD_CAST context);
  xmlNewProp(ctxt, BAD_CAST "name", BAD_CAST DOK_TREE_DECL(tree)->name);
  xmlNewProp(ctxt, BAD_CAST "type", BAD_CAST type_name);
  g_free(type_name);
  g_free(id);
  g_free(context);
}



/* leave_decl:
 */
static void leave_decl ( DokVisitor *visitor,
                         DokTree *tree )
{
  pop();
}
