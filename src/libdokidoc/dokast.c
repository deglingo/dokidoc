/* dokast.c -
 */

#include "libdokidoc/dokast.h"
#include <clog.h>
#include <string.h>



#define ASSERT(expr) do {                           \
    if (!(expr)) {                                  \
      CL_ERROR("ASSERTION FAILED: `" #expr "'");    \
    }                                               \
  } while (0)



typedef void (* DokASTInstanceInitFunc) ( DokAST *node );



/* DokASTTypeInfo:
 */
typedef struct _DokASTTypeInfo {
  guint instance_size;
  DokASTInstanceInitFunc instance_init;
  gchar * (* to_string) ( DokAST *node );
}
  DokASTTypeInfo;



/* TypeNode:
 */
typedef struct _TypeNode TypeNode;
struct _TypeNode
{
  DokASTNodeType type;
  gchar *name;
  TypeNode *parent;
  DokASTTypeInfo info;
};



static GHashTable *ident_cache = NULL;
static TypeNode type_nodes[DOK_AST_TYPE_COUNT];

static void dok_ast_node_init ( DokAST *node );
static gchar *dok_ast_node_to_string ( DokAST *node );



/* type_lookup:
 */
static inline TypeNode *type_lookup ( DokASTNodeType type )
{
  TypeNode *n;
  ASSERT(type > 0 && type < DOK_AST_TYPE_COUNT);
  n = &type_nodes[type];
  ASSERT(n->type > 0);
  return n;
}



/* dok_ast_type_register:
 */
static void dok_ast_type_register ( DokASTNodeType type,
                                    const gchar *name,
                                    DokASTNodeType parent,
                                    DokASTTypeInfo *info )
{
  TypeNode *node;
  ASSERT(type > 0 && type < DOK_AST_TYPE_COUNT);  
  node = &type_nodes[type];
  ASSERT(node->type == 0);
  node->type = type;
  node->name = g_strdup(name);
  ASSERT(parent >= 0 && parent < DOK_AST_TYPE_COUNT && parent != type);
  if (parent > 0) {
    node->parent = type_lookup(parent);
    ASSERT(info->instance_size >= node->parent->info.instance_size);
  } else {
    node->parent = NULL;
  }
  node->info = *info;
}



/* _dok_ast_init:
 */
void _dok_ast_init ( void )
{
  DokASTTypeInfo tp_node = {
    sizeof(DokASTNode),
    dok_ast_node_init,
    dok_ast_node_to_string,
  };
  DokASTTypeInfo tp_list = {
    sizeof(DokASTList),
    NULL,
  };
  DokASTTypeInfo tp_ident = {
    sizeof(DokASTIdent),
    NULL,
  };
  DokASTTypeInfo tp_keyword = {
    sizeof(DokASTKeyword),
    NULL,
  };
  DokASTTypeInfo tp_declarator = {
    sizeof(DokASTDeclarator),
    NULL,
  };
  DokASTTypeInfo tp_func_declarator = {
    sizeof(DokASTFuncDeclarator),
    NULL,
  };
  DokASTTypeInfo tp_decl = {
    sizeof(DokASTDecl),
    NULL,
  };
  DokASTTypeInfo tp_param_decl = {
    sizeof(DokASTParamDecl),
    NULL,
  };
  DokASTTypeInfo tp_func_decl = {
    sizeof(DokASTFuncDecl),
    NULL,
  };
  DokASTTypeInfo tp_var_decl = {
    sizeof(DokASTVarDecl),
    NULL,
  };
  DokASTTypeInfo tp_type = {
    sizeof(DokASTType),
    NULL,
  };
  DokASTTypeInfo tp_type_name = {
    sizeof(DokASTTypeName),
    NULL,
  };
  DokASTTypeInfo tp_type_builtin = {
    sizeof(DokASTTypeBuiltin),
    NULL,
  };
  DokASTTypeInfo tp_pointer = {
    sizeof(DokASTPointer),
    NULL,
  };
  DokASTTypeInfo tp_struct = {
    sizeof(DokASTStruct),
    NULL,
  };
  DokASTTypeInfo tp_enum = {
    sizeof(DokASTEnum),
    NULL,
  };
  ident_cache = g_hash_table_new(g_str_hash, g_str_equal);
  memset(type_nodes, 0, sizeof(TypeNode) * DOK_AST_TYPE_COUNT);

  dok_ast_type_register(DOK_AST_NODE, "Node", 0, &tp_node);
  dok_ast_type_register(DOK_AST_LIST, "List", DOK_AST_NODE, &tp_list);
  dok_ast_type_register(DOK_AST_IDENT, "Ident", DOK_AST_NODE, &tp_ident);
  dok_ast_type_register(DOK_AST_KEYWORD, "Keyword", DOK_AST_NODE, &tp_keyword);
  dok_ast_type_register(DOK_AST_DECLARATOR, "Declarator", DOK_AST_NODE, &tp_declarator);
  dok_ast_type_register(DOK_AST_FUNC_DECLARATOR, "FuncDeclarator", DOK_AST_DECLARATOR, &tp_func_declarator);
  dok_ast_type_register(DOK_AST_DECL, "Decl", DOK_AST_NODE, &tp_decl);
  dok_ast_type_register(DOK_AST_PARAM_DECL, "ParamDecl", DOK_AST_DECL, &tp_param_decl);
  dok_ast_type_register(DOK_AST_FUNC_DECL, "FuncDecl", DOK_AST_DECL, &tp_func_decl);
  dok_ast_type_register(DOK_AST_VAR_DECL, "VarDecl", DOK_AST_DECL, &tp_var_decl);
  dok_ast_type_register(DOK_AST_TYPE, "Type", DOK_AST_NODE, &tp_type);
  dok_ast_type_register(DOK_AST_TYPE_NAME, "TypeName", DOK_AST_TYPE, &tp_type_name);
  dok_ast_type_register(DOK_AST_TYPE_BUILTIN, "TypeBuiltin", DOK_AST_TYPE, &tp_type_builtin);
  dok_ast_type_register(DOK_AST_POINTER, "Pointer", DOK_AST_TYPE, &tp_pointer);
  dok_ast_type_register(DOK_AST_STRUCT, "Struct", DOK_AST_TYPE, &tp_struct);
  dok_ast_type_register(DOK_AST_ENUM, "Enum", DOK_AST_TYPE, &tp_enum);
}



/* dok_ast_type_isa:
 */
gboolean dok_ast_type_isa ( DokASTNodeType type1,
                            DokASTNodeType type2 )
{
  TypeNode *t1 = type_lookup(type1);
  TypeNode *t2 = type_lookup(type2);
  while (t1) {
    if (t1 == t2)
      return TRUE;
    t1 = t1->parent;
  }
  return FALSE;
}



/* dok_ast_cast:
 */
gpointer dok_ast_cast ( gpointer node,
                        DokASTNodeType type )
{
  if (dok_ast_check(node, type))
    return node;
  CL_ERROR("invalid node type");
  return NULL;
}



/* dok_ast_check:
 */
gboolean dok_ast_check ( gpointer node_,
                         DokASTNodeType type )
{
  DokASTNode *node = node_;
  return dok_ast_type_isa(node->type, type);
}



/* _node_init:
 */
static void _node_init ( DokASTNode *node,
                         DokASTNodeType type )
{
  TypeNode *type_node = type_lookup(type);
  if (type_node->parent)
    _node_init(node, type_node->parent->type);
  if (type_node->info.instance_init)
    type_node->info.instance_init(node);
}



/* dok_ast_new:
 */
static DokAST *dok_ast_new ( DokASTNodeType type )
{
  DokAST *node;
  TypeNode *type_node = type_lookup(type);
  node = g_malloc0(type_node->info.instance_size);
  node->type = type;
  _node_init(node, type);
  return node;
}



/* dok_ast_to_string:
 */
gchar *dok_ast_to_string ( DokAST *node )
{
  TypeNode *type;
  if (!node)
    return g_strdup("<NULL>");
  type = type_lookup(node->type);
  while (!type->info.to_string) {
    type = type->parent;
    ASSERT(type);
  }
  return type->info.to_string(node);
}



/* dok_ast_node_init:
 */
static void dok_ast_node_init ( DokAST *node )
{
}



/* dok_ast_node_to_string:
 */
static gchar *dok_ast_node_to_string ( DokAST *node )
{
  return g_strdup_printf("<%s %p>",
                         type_lookup(node->type)->name,
                         node);
}



/* dok_ast_list_append:
 */
DokAST *dok_ast_list_append ( DokAST *list,
                              DokAST *item )
{
  DokAST *link;
  ASSERT((!list) || DOK_AST_IS_LIST(list));
  link = dok_ast_new(DOK_AST_LIST);
  DOK_AST_LIST(link)->item = item;
  if (list)
    {
      DokAST *tail = dok_ast_list_tail(list);
      DOK_AST_LIST(tail)->next = link;
      DOK_AST_LIST(link)->prev = tail;
      return list;
    }
  else
    {
      return link;
    }
}



/* dok_ast_list_prepend:
 */
DokAST *dok_ast_list_prepend ( DokAST *list,
                               DokAST *item )
{
  DokAST *link;
  ASSERT((!list) || DOK_AST_IS_LIST(list));
  link = dok_ast_new(DOK_AST_LIST);
  DOK_AST_LIST(link)->item = item;
  if (list)
    {
      DOK_AST_LIST(list)->prev = link;
      DOK_AST_LIST(link)->next = list;
    }
  return link;
}



/* dok_ast_list_tail:
 */
DokAST *dok_ast_list_tail ( DokAST *list )
{
  if (!list)
    return NULL;
  ASSERT(DOK_AST_IS_LIST(list));
  while (DOK_AST_LIST_NEXT(list))
    list = DOK_AST_LIST_NEXT(list);
  return list;
}



/* dok_ast_list_merge:
 */
DokAST *dok_ast_list_merge ( DokAST *list1,
                             DokAST *list2 )
{
  DokAST *tail;
  ASSERT((!list1) || DOK_AST_IS_LIST(list1));
  ASSERT((!list2) || DOK_AST_IS_LIST(list2));
  if (!list1)
    return list2;
  if (!list2)
    return list1;
  tail = dok_ast_list_tail(list1);
  DOK_AST_LIST(tail)->next = list2;
  DOK_AST_LIST(list2)->prev = list1;
  return list1;
}



/* dok_ast_ident_new:
 */
DokAST *dok_ast_ident_new ( const gchar *name )
{
  DokAST *node;
  if (G_UNLIKELY(!(node = g_hash_table_lookup(ident_cache, name))))
    {
      node = dok_ast_new(DOK_AST_IDENT);
      DOK_AST_IDENT(node)->name = g_strdup(name);
      g_hash_table_insert(ident_cache, DOK_AST_IDENT_NAME(node), node);
    }
  return node;
}



/* dok_ast_keyword_new:
 */
DokAST *dok_ast_keyword_new ( gint token,
                              const gchar *name )
{
  DokAST *kw = dok_ast_new(DOK_AST_KEYWORD);
  DOK_AST_KEYWORD(kw)->token = token;
  DOK_AST_KEYWORD(kw)->name = g_strdup(name);
  return kw;
}



/* dok_ast_delcarator_new:
 */
DokAST *dok_ast_declarator_new ( DokAST *ident )
{
  DokAST *node = dok_ast_new(DOK_AST_DECLARATOR);
  DOK_AST_DECLARATOR(node)->ident = ident;
  return node;
}



/* dok_ast_declarator_set_param_list:
 */
void dok_ast_declarator_set_param_list ( DokAST *declarator,
                                         DokAST *param_list )
{
  ASSERT(DOK_AST_IS_DECLARATOR(declarator));
  ASSERT(!DOK_AST_DECLARATOR_PARAM_LIST(declarator));
  ASSERT(DOK_AST_IS_LIST(param_list));
  DOK_AST_DECLARATOR(declarator)->param_list = param_list;
}



/* dok_declarator_set_pointer:
 */
void dok_declarator_set_pointer ( DokAST *declarator,
                                  DokAST *pointer )
{
  ASSERT(DOK_AST_IS_DECLARATOR(declarator));
  ASSERT(!DOK_AST_DECLARATOR_TYPE(declarator)); /* [fixme] ?? */
  ASSERT(DOK_AST_IS_POINTER(pointer));
  DOK_AST_DECLARATOR(declarator)->type = pointer;
}



/* dok_ast_declarator_chain_type:
 */
DokAST *dok_ast_declarator_chain_type ( DokAST *declarator,
                                        DokAST *type )
{
  DokAST *dtype;
  ASSERT(DOK_AST_IS_DECLARATOR(declarator));
  ASSERT(DOK_AST_IS_TYPE(type));
  if ((dtype = DOK_AST_DECLARATOR_TYPE(declarator)))
    {
      ASSERT(DOK_AST_IS_POINTER(dtype));
      while (DOK_AST_POINTER_TARGET(dtype))
        {
          dtype = DOK_AST_POINTER_TARGET(dtype);
          ASSERT(DOK_AST_IS_POINTER(dtype));
        }
      DOK_AST_POINTER(dtype)->target = type;
      return dtype;
    }
  else
    {
      return type;
    }
}



/* dok_ast_func_declarator_new:
 */
DokAST *dok_ast_func_declarator_new ( DokAST *target )
{
  DokAST *node = dok_ast_new(DOK_AST_FUNC_DECLARATOR);
  DOK_AST_FUNC_DECLARATOR(node)->target = target;
  return node;
}



/* dok_ast_decl_new:
 */
DokAST *dok_ast_decl_new ( DokASTNodeType node_type,
                           DokAST *type,
                           DokAST *declarator )
{
  DokAST *node;
  ASSERT(dok_ast_type_isa(node_type, DOK_AST_DECL));
  ASSERT((!type) || DOK_AST_IS_TYPE(type));
  ASSERT((!declarator) || DOK_AST_IS_DECLARATOR(declarator));
  node = dok_ast_new(node_type);
  CL_DEBUG("NEW DECL:");
  CL_DEBUG("  type: %s", dok_ast_to_string(type));
  CL_DEBUG("  declarator: %s", dok_ast_to_string(declarator));
  CL_DEBUG("[TODO]");
  return node;
}



/* dok_ast_param_decl_new:
 */
DokAST *dok_ast_param_decl_new ( DokAST *type_spec,
                                 DokAST *declarator )
{
  DokAST *node;
  node = dok_ast_decl_new(DOK_AST_PARAM_DECL, type_spec, declarator);
  return node;
}



/* dok_ast_func_decl_new:
 */
DokAST *dok_ast_func_decl_new ( DokAST *type_spec,
                                DokAST *declarator )
{
  DokAST *node;
  node = dok_ast_decl_new(DOK_AST_FUNC_DECL, type_spec, declarator);
  DOK_AST_FUNC_DECL(node)->param_list = DOK_AST_DECLARATOR_PARAM_LIST(declarator);
  return node;
}



/* dok_ast_var_decl_new:
 */
DokAST *dok_ast_var_decl_new ( DokAST *type,
                               DokAST *ident )
{
  DokAST *node = dok_ast_new(DOK_AST_VAR_DECL);
  /* [FIXME] */
  DOK_AST_DECL(node)->type = type;
  DOK_AST_DECL(node)->ident = ident;
  return node;
}



/* dok_ast_type_name_new:
 */
DokAST *dok_ast_type_name_new ( DokAST *ident )
{
  DokAST *node;
  ASSERT(DOK_AST_IS_IDENT(ident));
  node = dok_ast_new(DOK_AST_TYPE_NAME);
  DOK_AST_TYPE_NAME(node)->ident = ident;
  return node;
}



/* dok_ast_type_builtin_new:
 */
DokAST *dok_ast_type_builtin_new ( void )
{
  DokAST *node;
  node = dok_ast_new(DOK_AST_TYPE_BUILTIN);
  return node;
}



/* dok_ast_pointer_new:
 */
DokAST *dok_ast_pointer_new ( DokAST *target )
{
  DokAST *node;
  ASSERT((!target) || DOK_AST_IS_TYPE(target));
  node = dok_ast_new(DOK_AST_POINTER);
  DOK_AST_POINTER(node)->target = target;
  return node;
}



/* dok_ast_struct_new:
 */
DokAST *dok_ast_struct_new ( void )
{
  DokAST *node;
  node = dok_ast_new(DOK_AST_STRUCT);
  return node;
}



/* dok_ast_enum_new:
 */
DokAST *dok_ast_enum_new ( void )
{
  DokAST *node;
  node = dok_ast_new(DOK_AST_ENUM);
  return node;
}
