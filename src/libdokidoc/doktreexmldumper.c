/* doktreexmldumper.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktreexmldumper.h"



/* [FIXME] 22 for 64bits */
#define MAX_INT_CHARS 32



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
static void enter_loc ( DokVisitor *visitor,
                        DokTree *tree );
static void leave_loc ( DokVisitor *visitor,
                        DokTree *tree );
static void enter_file ( DokVisitor *visitor,
                         DokTree *tree );
static void leave_file ( DokVisitor *visitor,
                         DokTree *tree );
static void enter_namespace ( DokVisitor *visitor,
                              DokTree *tree );
static void leave_namespace ( DokVisitor *visitor,
                              DokTree *tree );
static void enter_var ( DokVisitor *visitor,
                        DokTree *tree );
static void leave_var ( DokVisitor *visitor,
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
      REG(LOC, loc);
      REG(FILE, file);
      REG(NAMESPACE, namespace);
      REG(VAR, var);
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
  CL_ERROR("[TODO] enter(%s)", dok_tree_to_string(node));
}



/* enter_root:
 */
static void enter_root ( DokVisitor *visitor,
                         DokTree *tree )
{
  ASSERT(!proc->root);
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



/* enter_loc:
 */
static void enter_loc ( DokVisitor *visitor,
                        DokTree *tree )
{
  gchar *lineno, *fid;
  DOK_APRINTF(&lineno, "%d", DOK_TREE_LOC_LINENO(tree));
  DOK_APRINTF(&fid, "%d", DOK_TREE_ITEM_ID(DOK_TREE_LOC_FILE(tree)));
  push(xmlNewChild(ctxt, NULL, BAD_CAST "location", NULL));
  xmlNewProp(ctxt, BAD_CAST "file", BAD_CAST fid);
  xmlNewProp(ctxt, BAD_CAST "lineno", BAD_CAST lineno);
  xmlNewProp(ctxt, BAD_CAST "isdef", BAD_CAST (DOK_TREE_LOC_ISDEF(tree) ? "1" : "0"));
}



/* leave_loc:
 */
static void leave_loc ( DokVisitor *visitor,
                        DokTree *tree )
{
  pop();
}



/* enter_item:
 */
static void enter_item ( DokVisitor *visitor,
                         DokTree *tree )
{
  gchar *id;
  DOK_APRINTF(&id, "%d", DOK_TREE_ITEM_ID(tree));
  xmlNewProp(ctxt, BAD_CAST "name", BAD_CAST DOK_TREE_ITEM_NAME(tree));
  xmlNewProp(ctxt, BAD_CAST "id", BAD_CAST id);
}



/* enter_file:
 */
static void enter_file ( DokVisitor *visitor,
                         DokTree *tree )
{
  push(xmlNewChild(ctxt, NULL, BAD_CAST "file", NULL));
  enter_item(visitor, tree);
}



/* leave_file:
 */
static void leave_file ( DokVisitor *visitor,
                         DokTree *tree )
{
  pop();
}



/* enter_decl:
 */
static void enter_decl ( DokVisitor *visitor,
                         DokTree *tree )
{
  gchar *context;
  enter_item(visitor, tree);
  DOK_APRINTF(&context, "%d", DOK_TREE_DECL_CONTEXT_ID(tree));
  xmlNewProp(ctxt, BAD_CAST "context", BAD_CAST context);
}



/* enter_namespace:
 */
static void enter_namespace ( DokVisitor *visitor,
                              DokTree *tree )
{
  push(xmlNewChild(proc->root, NULL, BAD_CAST "namespace", NULL));
  enter_decl(visitor, tree);
}



/* leave_namespace:
 */
static void leave_namespace ( DokVisitor *visitor,
                              DokTree *tree )
{
  pop();
}



/* enter_var:
 */
static void enter_var ( DokVisitor *visitor,
                        DokTree *tree )
{
  push(xmlNewChild(proc->root, NULL, BAD_CAST "var", NULL));
  enter_decl(visitor, tree);  
}



/* leave_var:
 */
static void leave_var ( DokVisitor *visitor,
                        DokTree *tree )
{
  pop();
}
