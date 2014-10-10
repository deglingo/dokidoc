/* dokast.h -
 */

#ifndef _DOKAST_H_
#define _DOKAST_H_

#include "libdokidoc/dokbase.h"



typedef enum _DokASTType DokASTType;
typedef union _DokAST DokAST;
typedef struct _DokASTList DokASTList;
typedef struct _DokASTKeyword DokASTKeyword;
typedef struct _DokASTIdent DokASTIdent;
typedef struct _DokASTTypeSpec DokASTTypeSpec;
typedef struct _DokASTDeclarator DokASTDeclarator;
typedef struct _DokASTParam DokASTParam;
typedef struct _DokASTFunc DokASTFunc;



#define DOK_AST_HEADER \
  DokASTType type



/* DokASTType:
 */
enum _DokASTType
  {
    DOK_AST_LIST,
    DOK_AST_KEYWORD,
    DOK_AST_IDENT,
    DOK_AST_TYPE_SPEC,
    DOK_AST_DECLARATOR,
    DOK_AST_PARAM,
    DOK_AST_FUNC,
    DOK_AST_DECL,
  };



/* DokASTList:
 */
struct _DokASTList
{
  DOK_AST_HEADER;
  DokAST *item;
  DokAST *next;
  DokAST *prev;
};



/* DokASTKeyword:
 */
struct _DokASTKeyword
{
  DOK_AST_HEADER;
  gint token;
  gchar *name;
};



/* DokASTIdent:
 */
struct _DokASTIdent
{
  DOK_AST_HEADER;
  gchar *name;
};



/* DokASTTypeSpec:
 */
struct _DokASTTypeSpec
{
  DOK_AST_HEADER;
  DokAST *specifiers;
  DokAST *type_name;
};



/* DokASTDeclarator:
 */
struct _DokASTDeclarator
{
  DOK_AST_HEADER;
  DokAST *ident;
};



/* DokASTParam:
 */
struct _DokASTParam
{
  DOK_AST_HEADER;
  DokAST *type_spec;
  DokAST *declarator;
};



/* DokASTFunc:
 */
struct _DokASTFunc
{
  DOK_AST_HEADER;
  DokAST *type_spec;
  DokAST *declarator;
  DokAST *param_list;
};



/* DokASTDecl:
 */
struct _DokASTDecl
{
  DOK_AST_HEADER;
  DokAST *type_spec;
  DokAST *declarator;
};



/* DokAST:
 */
union _DokAST
{
  DokASTType type;
  DokASTList list;
  DokASTKeyword keyword;
  DokASTIdent ident;
  DokASTTypeSpec type_spec;
  DokASTDeclarator declarator;
  DokASTParam param;
  DokASTFunc func;
};



DokAST *dok_ast_list_append ( DokAST *list,
                              DokAST *item );
DokAST *dok_ast_list_prepend ( DokAST *list,
                               DokAST *item );
DokAST *dok_ast_list_tail ( DokAST *list );
DokAST *dok_ast_keyword_new ( gint token,
                              const gchar *name );
DokAST *dok_ast_ident_new ( const gchar *name );
DokAST *dok_ast_type_spec_new ( DokAST *specifiers,
                                DokAST *type_name );
DokAST *dok_ast_declarator_new ( DokAST *ident );
DokAST *dok_ast_param_new ( DokAST *type_spec,
                            DokAST *declarator );
DokAST *dok_ast_func_new ( DokAST *type_spec,
                           DokAST *declarator,
                           DokAST *param_list );


#endif
