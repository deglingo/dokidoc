/* dokast.c -
 */

#include "libdokidoc/dokast.h"



static GHashTable *ident_cache = NULL;



/* dok_ast_new:
 */
static DokAST *dok_ast_new ( DokASTType type )
{
  DokAST *node = g_new0(DokAST, 1);
  node->type = type;
  return node;
}



/* dok_ast_list_append:
 */
DokAST *dok_ast_list_append ( DokAST *list,
                              DokAST *item )
{
  DokAST *node = dok_ast_new(DOK_AST_LIST);
  node->list.item = item;
  if (list)
    {
      DokAST *tail = dok_ast_list_tail(list);
      tail->list.next = node;
      node->list.prev = tail;
      return list;
    }
  else
    {
      return node;
    }
}



/* dok_ast_list_prepend:
 */
DokAST *dok_ast_list_prepend ( DokAST *list,
                               DokAST *item )
{
  DokAST *node = dok_ast_new(DOK_AST_LIST);
  node->list.item = item;
  if (list)
    {
      list->list.prev = node;
      node->list.next = list;
      return node;
    }
  else
    {
      return node;
    }
}



/* dok_ast_list_tail:
 */
DokAST *dok_ast_list_tail ( DokAST *list )
{
  if (!list)
    return NULL;
  while (list->list.next)
    list = list->list.next;
  return list;
}



/* dok_ast_keyword_new:
 */
DokAST *dok_ast_keyword_new ( gint token,
                              const gchar *name )
{
  DokAST *ast = dok_ast_new(DOK_AST_KEYWORD);
  ast->keyword.token = token;
  ast->keyword.name = g_strdup(name);
  return ast;
}



/* dok_ast_ident_new:
 */
DokAST *dok_ast_ident_new ( const gchar *name )
{
  DokAST *node;
  /* [fixme] global init */
  if (!ident_cache)
    ident_cache = g_hash_table_new(g_str_hash, g_str_equal);
  if (G_UNLIKELY(!(node = g_hash_table_lookup(ident_cache, name))))
    {
      node = dok_ast_new(DOK_AST_IDENT);
      node->ident.name = g_strdup(name);
      g_hash_table_insert(ident_cache, node->ident.name, node);
    }
  return node;
}



/* dok_ast_type_spec_new:
 */
DokAST *dok_ast_type_spec_new ( DokAST *specifiers,
                                DokAST *type_name )
{
  DokAST *node = dok_ast_new(DOK_AST_TYPE_SPEC);
  node->type_spec.specifiers = specifiers;
  node->type_spec.type_name = type_name;
  return node;
}



/* dok_ast_delcarator:
 */
DokAST *dok_ast_declarator_new ( DokAST *ident )
{
  DokAST *node = dok_ast_new(DOK_AST_DECLARATOR);
  node->declarator.ident = ident;
  return node;
}



/* dok_ast_param_new:
 */
DokAST *dok_ast_param_new ( DokAST *type_spec,
                            DokAST *declarator )
{
  DokAST *node = dok_ast_new(DOK_AST_PARAM);
  node->param.type_spec = type_spec;
  node->param.declarator = declarator;
  return node;
}



/* dok_ast_func_new:
 */
DokAST *dok_ast_func_new ( DokAST *type_spec,
                           DokAST *declarator,
                           DokAST *param_list )
{
  DokAST *node = dok_ast_new(DOK_AST_FUNC);
  node->func.type_spec = type_spec;
  node->func.declarator = declarator;
  node->func.param_list = param_list;
  return node;
}
