/* dokast.h -
 */

#ifndef _DOKAST_H_
#define _DOKAST_H_

#include "libdokidoc/dokbase.h"

struct _DokASTVisitor;



typedef enum _DokASTNodeType DokASTNodeType;
typedef struct _DokASTNode DokASTNode;
typedef DokASTNode DokAST; /* [FIXME] */
typedef struct _DokASTList DokASTList;
typedef struct _DokASTIdent DokASTIdent;
typedef struct _DokASTKeyword DokASTKeyword;
typedef struct _DokASTNamespace DokASTNamespace;
typedef struct _DokASTDeclarator DokASTDeclarator;
typedef struct _DokASTFuncDeclarator DokASTFuncDeclarator;
typedef struct _DokASTDecl DokASTDecl;
typedef struct _DokASTParamDecl DokASTParamDecl;
typedef struct _DokASTFuncDecl DokASTFuncDecl;
typedef struct _DokASTVarDecl DokASTVarDecl;
typedef struct _DokASTFunc DokASTFunc;
typedef struct _DokASTType DokASTType;
typedef struct _DokASTTypeName DokASTTypeName;
typedef struct _DokASTTypeBuiltin DokASTTypeBuiltin;
typedef struct _DokASTPointer DokASTPointer;
typedef struct _DokASTStruct DokASTStruct;
typedef struct _DokASTEnum DokASTEnum;



/* DokASTNodeType:
 */
enum _DokASTNodeType
  {
    DOK_AST_NONE = 0,
    DOK_AST_NODE,
    DOK_AST_LIST,
    DOK_AST_IDENT,
    DOK_AST_NAMESPACE,
    DOK_AST_KEYWORD,
    DOK_AST_DECLARATOR,
    DOK_AST_FUNC_DECLARATOR,
    DOK_AST_DECL,
    DOK_AST_PARAM_DECL,
    DOK_AST_FUNC_DECL,
    DOK_AST_VAR_DECL,
    DOK_AST_FUNC,
    DOK_AST_TYPE,
    DOK_AST_TYPE_NAME,
    DOK_AST_TYPE_BUILTIN,
    DOK_AST_POINTER,
    DOK_AST_STRUCT,
    DOK_AST_ENUM,

    DOK_AST_TYPE_COUNT,
  };



/* DokASTNode:
 */
struct _DokASTNode
{
  DokASTNodeType type;
};



/* DokASTList:
 */
struct _DokASTList
{
  DokASTNode super;

  DokAST *item;
  DokAST *next;
  DokAST *prev;
};



/* DokASTIdent:
 */
struct _DokASTIdent
{
  DokASTNode super;

  gchar *name;
};



/* DokASTKeyword:
 */
struct _DokASTKeyword
{
  DokASTNode super; /* [FIXME] Ident ? */

  gint token;
  gchar *name;
};



/* DokASTDeclarator:
 */
struct _DokASTDeclarator
{
  DokASTNode super;

  DokAST *ident;
  DokAST *type;
  DokAST *param_list;
};



/* DokASTFuncDeclarator:
 */
struct _DokASTFuncDeclarator
{
  DokASTDeclarator super;

  DokAST *target;
};



/* DokASTDecl:
 */
struct _DokASTDecl
{
  DokASTNode super;

  DokAST *type;
  DokAST *ident;
  DokLocation loc;
};



/* DokASTNamespace:
 */
struct _DokASTNamespace
{
  DokASTDecl super;

  DokAST *decls;
};



/* DokASTParamDecl:
 */
struct _DokASTParamDecl
{
  DokASTDecl super;
};



/* DokASTFuncDecl:
 */
struct _DokASTFuncDecl
{
  DokASTDecl super;

  DokAST *param_list;
};



/* DokASTVarDecl:
 */
struct _DokASTVarDecl
{
  DokASTDecl super;
};



/* DokASTType:
 */
struct _DokASTType
{
  DokASTNode super;
};



/* DokASTTypeName:
 */
struct _DokASTTypeName
{
  DokASTType super;

  DokAST *ident;
};



/* DokASTTypeBuiltin:
 */
struct _DokASTTypeBuiltin
{
  DokASTType super;

  /* [fixme] type */
};



/* DokASTPointer:
 */
struct _DokASTPointer
{
  DokASTType super;

  DokAST *target;
};



/* DokASTStruct:
 */
struct _DokASTStruct
{
  DokASTType super;
};



/* DokASTEnum:
 */
struct _DokASTEnum
{
  DokASTType super;
};



void _dok_ast_init ( void );

#define DOK_AST_CAST(node, type, s_type) ((s_type *) dok_ast_cast((node), (type)))
#define DOK_AST_CHECK(node, type) (dok_ast_check((node), (type)))

gboolean dok_ast_type_isa ( DokASTNodeType type1,
                            DokASTNodeType type2 );
gpointer dok_ast_cast ( gpointer node,
                        DokASTNodeType type );
gboolean dok_ast_check ( gpointer node,
                         DokASTNodeType type );

gchar *dok_ast_to_string ( DokAST *node );

/* DokASTList */
#define DOK_AST_LIST(n)    (DOK_AST_CAST((n), DOK_AST_LIST, DokASTList))
#define DOK_AST_IS_LIST(n) (DOK_AST_CHECK((n), DOK_AST_LIST))

#define DOK_AST_LIST_ITEM(n) (DOK_AST_LIST(n)->item)
#define DOK_AST_LIST_NEXT(n) (DOK_AST_LIST(n)->next)
#define DOK_AST_LIST_PREV(n) (DOK_AST_LIST(n)->prev)

DokAST *dok_ast_list_append ( DokAST *list,
                              DokAST *item );
DokAST *dok_ast_list_prepend ( DokAST *list,
                               DokAST *item );
DokAST *dok_ast_list_tail ( DokAST *list );
DokAST *dok_ast_list_merge ( DokAST *list1,
                             DokAST *list2 );

/* DokASTIdent */
#define DOK_AST_IDENT(n)    (DOK_AST_CAST((n), DOK_AST_IDENT, DokASTIdent))
#define DOK_AST_IS_IDENT(n) (DOK_AST_CHECK((n), DOK_AST_IDENT))

#define DOK_AST_IDENT_NAME(n) (DOK_AST_IDENT(n)->name)

DokAST *dok_ast_ident_new ( const gchar *name );

/* DokASTKeyword */
#define DOK_AST_KEYWORD(n)    (DOK_AST_CAST((n), DOK_AST_KEYWORD, DokASTKeyword))
#define DOK_AST_IS_KEYWORD(n) (DOK_AST_CHECK((n), DOK_AST_KEYWORD))

#define DOK_AST_KEYWORD_TOKEN(n) (DOK_AST_KEYWORD(n)->token)
#define DOK_AST_KEYWORD_NAME(n) (DOK_AST_KEYWORD(n)->name)

DokAST *dok_ast_keyword_new ( gint token,
                              const gchar *name );

/* DokASTNamespace */
#define DOK_AST_NAMESPACE(n)    (DOK_AST_CAST((n), DOK_AST_NAMESPACE, DokASTNamespace))
#define DOK_AST_IS_NAMESPACE(n) (DOK_AST_CHECK((n), DOK_AST_NAMESPACE))

#define DOK_AST_NAMESPACE_DECLS(n) (DOK_AST_NAMESPACE(n)->decls)

DokAST *dok_ast_namespace_new ( void );
void dok_ast_namespace_add_decls ( DokAST *namespace,
                                   DokAST *decls );

/* DokASTDeclarator */
#define DOK_AST_DECLARATOR(n)    (DOK_AST_CAST((n), DOK_AST_DECLARATOR, DokASTDeclarator))
#define DOK_AST_IS_DECLARATOR(n) (DOK_AST_CHECK((n), DOK_AST_DECLARATOR))

#define DOK_AST_DECLARATOR_IDENT(n) (DOK_AST_DECLARATOR(n)->ident)
#define DOK_AST_DECLARATOR_PARAM_LIST(n) (DOK_AST_DECLARATOR(n)->param_list)
#define DOK_AST_DECLARATOR_TYPE(n) (DOK_AST_DECLARATOR(n)->type)

DokAST *dok_ast_declarator_new ( DokAST *ident );
void dok_ast_declarator_set_param_list ( DokAST *declarator,
                                         DokAST *param_list );
void dok_declarator_set_pointer ( DokAST *declarator,
                                  DokAST *pointer );
DokAST *dok_ast_declarator_chain_type ( DokAST *declarator,
                                        DokAST *type );

/* DokASTFuncDeclarator */
#define DOK_AST_FUNC_DECLARATOR(n)    (DOK_AST_CAST((n), DOK_AST_FUNC_DECLARATOR, DokASTFuncDeclarator))
#define DOK_AST_IS_FUNC_DECLARATOR(n) (DOK_AST_CHECK((n), DOK_AST_FUNC_DECLARATOR))

#define DOK_AST_FUNC_DECLARATOR_TARGET(n) (DOK_AST_FUNC_DECLARATOR(n)->target)

DokAST *dok_ast_func_declarator_new ( DokAST *target );

/* DokASTDecl */
#define DOK_AST_DECL(n)    (DOK_AST_CAST((n), DOK_AST_DECL, DokASTDecl))
#define DOK_AST_IS_DECL(n) (DOK_AST_CHECK((n), DOK_AST_DECL))

#define DOK_AST_DECL_TYPE(n) (DOK_AST_DECL(n)->type)
#define DOK_AST_DECL_IDENT(n) (DOK_AST_DECL(n)->ident)
#define DOK_AST_DECL_NAME(n) (DOK_AST_DECL_IDENT(n) ? DOK_AST_IDENT_NAME(DOK_AST_DECL_IDENT(n)) : "??")
#define DOK_AST_DECL_LOC(n) (DOK_AST_DECL(n)->loc)

DokAST *dok_ast_decl_new ( DokASTNodeType type,
                           DokAST *type_spec,
                           DokAST *declarator );

/* DokASTParamDecl */
#define DOK_AST_PARAM_DECL(n)    (DOK_AST_CAST((n), DOK_AST_PARAM_DECL, DokASTParamDecl))
#define DOK_AST_IS_PARAM_DECL(n) (DOK_AST_CHECK((n), DOK_AST_PARAM_DECL))

DokAST *dok_ast_param_decl_new ( DokAST *type_spec,
                                 DokAST *declarator );

/* DokASTFuncDecl */
#define DOK_AST_FUNC_DECL(n)    (DOK_AST_CAST((n), DOK_AST_FUNC_DECL, DokASTFuncDecl))
#define DOK_AST_IS_FUNC_DECL(n) (DOK_AST_CHECK((n), DOK_AST_FUNC_DECL))

DokAST *dok_ast_func_decl_new ( DokAST *type_spec,
                                DokAST *declarator );

/* DokASTVarDecl */
#define DOK_AST_VAR_DECL(n)    (DOK_AST_CAST((n), DOK_AST_VAR_DECL, DokASTVarDecl))
#define DOK_AST_IS_VAR_DECL(n) (DOK_AST_CHECK((n), DOK_AST_VAR_DECL))

DokAST *dok_ast_var_decl_new ( DokAST *type,
                               DokAST *ident,
                               DokLocation *loc );

/* DokASTType */
#define DOK_AST_TYPE(n)    (DOK_AST_CAST((n), DOK_AST_TYPE, DokASTType))
#define DOK_AST_IS_TYPE(n) (DOK_AST_CHECK((n), DOK_AST_TYPE))

/* DokASTTypeName */
#define DOK_AST_TYPE_NAME(n)    (DOK_AST_CAST((n), DOK_AST_TYPE_NAME, DokASTTypeName))
#define DOK_AST_IS_TYPE_NAME(n) (DOK_AST_CHECK((n), DOK_AST_TYPE_NAME))

#define DOK_AST_TYPE_NAME_IDENT(n) (DOK_AST_TYPE_NAME(n)->ident)
#define DOK_AST_TYPE_NAME_NAME(n) (DOK_AST_IDENT_NAME(DOK_AST_TYPE_NAME_IDENT(n)))

DokAST *dok_ast_type_name_new ( DokAST *ident );

/* DokASTTypeBuiltin */
#define DOK_AST_TYPE_BUILTIN(n)    (DOK_AST_CAST((n), DOK_AST_TYPE_BUILTIN, DokASTTypeBuiltin))
#define DOK_AST_IS_TYPE_BUILTIN(n) (DOK_AST_CHECK((n), DOK_AST_TYPE_BUILTIN))

DokAST *dok_ast_type_builtin_new ( void );

/* DokASTPointer */
#define DOK_AST_POINTER(n)    (DOK_AST_CAST((n), DOK_AST_POINTER, DokASTPointer))
#define DOK_AST_IS_POINTER(n) (DOK_AST_CHECK((n), DOK_AST_POINTER))

#define DOK_AST_POINTER_TARGET(n) (DOK_AST_POINTER(n)->target)

DokAST *dok_ast_pointer_new ( DokAST *target );

/* DokASTStruct */
#define DOK_AST_STRUCT(n)    (DOK_AST_CAST((n), DOK_AST_STRUCT, DokASTStruct))
#define DOK_AST_IS_STRUCT(n) (DOK_AST_CHECK((n), DOK_AST_STRUCT))

DokAST *dok_ast_struct_new ( void );

/* DokASTEnum */
#define DOK_AST_ENUM(n)    (DOK_AST_CAST((n), DOK_AST_ENUM, DokASTEnum))
#define DOK_AST_IS_ENUM(n) (DOK_AST_CHECK((n), DOK_AST_ENUM))

DokAST *dok_ast_enum_new ( void );



#endif
