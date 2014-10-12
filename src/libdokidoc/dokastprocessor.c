/* dokastdumper.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokastprocessor.h"



/* DokASTProcessor:
 */
typedef struct _DokASTProcessor
{
  DokASTVisitor visitor;

  xmlNodePtr root;
  GSList *context;
}
  DokASTProcessor;



static void enter_list ( DokVisitor *v,
                         DokAST *n );
static void leave_list ( DokVisitor *v,
                         DokAST *n );
static void enter_unit ( DokVisitor *v,
                         DokAST *n );
static void leave_unit ( DokVisitor *v,
                         DokAST *n );
static void enter_var_decl ( DokVisitor *v,
                             DokAST *n );
static void leave_var_decl ( DokVisitor *v,
                             DokAST *n );
static void enter_type_name ( DokVisitor *v,
                              DokAST *n );
static void leave_type_name ( DokVisitor *v,
                              DokAST *n );
static void enter_pointer ( DokVisitor *v,
                            DokAST *n );
static void leave_pointer ( DokVisitor *v,
                            DokAST *n );



/* dok_ast_processor_get_class:
 */
static DokVisitorClass *dok_ast_processor_get_class ( void )
{
  static DokVisitorClass *cls = NULL;
  if (!cls)
    {
      cls = dok_ast_visitor_class_new();
      cls->instance_size = sizeof(DokASTProcessor);
#define REG(tp, name) dok_visitor_class_register_funcs  \
        (cls,                                           \
         GUINT_TO_POINTER(tp),                          \
         NULL,                                          \
         (DokVisitorFunc) enter_##name,                 \
         (DokVisitorFunc) leave_##name)
      REG(DOK_AST_LIST, list);
      REG(DOK_AST_UNIT, unit);
      REG(DOK_AST_VAR_DECL, var_decl);
      REG(DOK_AST_TYPE_NAME, type_name);
      REG(DOK_AST_POINTER, pointer);
#undef REG
    }
  return cls;
}



/* dok_ast_processor_new:
 */
DokVisitor *dok_ast_processor_new ( void )
{
  DokVisitor *v = dok_visitor_new(dok_ast_processor_get_class());
  DokASTProcessor *p = (DokASTProcessor *) v;
  /* p->root = dok_tree_new(); */
  /* p->context = g_slist_prepend(NULL, p->root); */
  return v;
}



/* dok_ast_processor_get_tree:
 */
DokTree *dok_ast_processor_get_tree ( DokVisitor *processor )
{
  CL_ERROR("[TODO]");
  return NULL;
}



#define proc ((DokASTProcessor *) v)
#define ctxt ((DokTree *) (proc->context->data))
#define push(n) (proc->context = g_slist_prepend(proc->context, (n)))
#define pop() (proc->context = g_slist_delete_link(proc->context, proc->context))



/* enter_list:
 */
static void enter_list ( DokVisitor *v,
                         DokAST *n )
{
}



/* leave_list:
 */
static void leave_list ( DokVisitor *v,
                         DokAST *n )
{
  CL_TRACE("%s", dok_ast_to_string(n));
}



/* enter_unit:
 */
static void enter_unit ( DokVisitor *v,
                         DokAST *n )
{
  CL_TRACE("%s", dok_ast_to_string(n));
  /* push(dok_tree_new_namespace(ctxt, "")); */
}



/* leave_unit:
 */
static void leave_unit ( DokVisitor *v,
                         DokAST *n )
{
  CL_TRACE("%s", dok_ast_to_string(n));
  /* pop(); */
}



/* enter_var_decl:
 */
static void enter_var_decl ( DokVisitor *v,
                             DokAST *n )
{
  CL_TRACE("%s", dok_ast_to_string(n));
  /* push(dok_tree_var(ctxt, DOK_AST_DECL_NAME(n))); */
}



/* leave_var_decl:
 */
static void leave_var_decl ( DokVisitor *v,
                             DokAST *n )
{
  CL_TRACE("%s", dok_ast_to_string(n));
  /* pop(); */
}



/* enter_type_name:
 */
static void enter_type_name ( DokVisitor *v,
                              DokAST *n )
{
  CL_TRACE("%s", dok_ast_to_string(n));
  /* push(xmlNewChild(ctxt, NULL, BAD_CAST "type", BAD_CAST DOK_AST_TYPE_NAME_NAME(n))); */
  /* xmlNewProp(ctxt, BAD_CAST "class", BAD_CAST "type-name"); */
}



/* leave_type_name:
 */
static void leave_type_name ( DokVisitor *v,
                              DokAST *n )
{
  CL_TRACE("%s", dok_ast_to_string(n));
  /* pop(); */
}



/* enter_pointer:
 */
static void enter_pointer ( DokVisitor *v,
                            DokAST *n )
{
  CL_TRACE("%s", dok_ast_to_string(n));
  /* push(xmlNewChild(ctxt, NULL, BAD_CAST "type", NULL)); */
  /* xmlNewProp(ctxt, BAD_CAST "class", BAD_CAST "pointer"); */
}



/* leave_pointer:
 */
static void leave_pointer ( DokVisitor *v,
                            DokAST *n )
{
  CL_TRACE("%s", dok_ast_to_string(n));
  /* pop(); */
}
