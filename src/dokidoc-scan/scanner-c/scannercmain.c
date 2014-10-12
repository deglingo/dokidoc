/* scannercmain.c -
 */

#include "libdokidoc/libdokidoc.h"
#include "scannerc.h"
#include "scannercgram.h"
#include "cpp.h"
#include <glib.h>
#include <clog.h>
#include <stdio.h>



/* DokScannerC:
 */
typedef struct _DokScannerC
{
  DokScanner dok_scanner;
  CPP *cpp;
  DokAST *unit;
}
  DokScannerC;



static GHashTable *keywords = NULL;

struct keyword
{
  gint token;
  gchar *name;
};

static struct keyword KEYWORDS_LIST[] =
  {
    { TOK_STATIC, "static" },
    { TOK_STRUCT, "struct" },
    { TOK_UNION, "union" },
    { TOK_ENUM, "enum" },
    { TOK_VOID, "void" },
    { TOK_CONST, "const" },
    { TOK_INLINE, "inline" },
    { TOK_TYPEDEF, "typedef" },
    { 0, },
  };



/* _scanner_new:
 */
static DokScanner *_scanner_new ( void )
{
  DokScannerC *scanner;
  scanner = g_new0(DokScannerC, 1);
  scanner->unit = dok_ast_unit_new();
  return (DokScanner *) scanner;
}



/* _scanner_process:
 */
static DokAST *_scanner_process ( DokScanner *scanner,
                                  const gchar *filename,
                                  GError **error )
{
  DokScannerC *cscanner = (DokScannerC *) scanner;
  cscanner->cpp = cpp_new(filename);
  if (yyparse(scanner) != 0)
    CL_ERROR("parse failed");
  return cscanner->unit;
}



/* dokidoc_scanner_module_init:
 */
void dokidoc_scanner_module_init ( DokScannerFuncs *funcs )
{
  CL_DEBUG("SCAN PROCESS...");
  if (!keywords)
    {
      struct keyword *kw;
      keywords = g_hash_table_new(g_str_hash, g_str_equal);
      for (kw = KEYWORDS_LIST; kw->token; kw++)
        g_hash_table_insert(keywords, kw->name, dok_ast_keyword_new(kw->token, kw->name));
    }
  funcs->scanner_new = _scanner_new;
  funcs->scanner_process = _scanner_process;
}



/* yylex:
 */
int yylex ( DokAST **lvalp,
            Loc *llocp,
            DokScanner *scanner )
{
  Token *tok;
  /* LexRule *rule; */
  /* for (rule = LEX_RULES;  */
  /* CL_TRACE("%p", scanner); */
  while (1)
    {
      if (!(tok = cpp_lex(((DokScannerC *) scanner)->cpp)))
        return 0;
      CL_DEBUG("TOKEN: %d (%s)", tok->type, tok->value);
      switch (tok->type)
        {
        case TOK_COMMENT:
          CL_DEBUG("[TODO] comment: `%s'", tok->value);
          continue;
        case TOK_IDENT:
          {
            DokAST *kw;
            if ((kw = g_hash_table_lookup(keywords, tok->value))) {
              *lvalp = kw;
              return DOK_AST_KEYWORD_TOKEN(kw);
            } else {
              *lvalp = dok_ast_ident_new(tok->value);
              return TOK_IDENT;
            }
          }
        default:
          *lvalp = NULL;
          return tok->type;
        }
    }
}



/* yyerror:
 */
void yyerror ( Loc *locp,
               DokScanner *scanner,
               char const *msg )
{
  CL_DEBUG("ERROR: %s", msg);
}



/* eat_function_body:
 */
void eat_function_body ( DokScanner *scanner )
{
  Token *tok;
  gint depth = 1;
  CL_TRACE("%p", scanner);
  while (1)
    {
      if (!(tok = cpp_lex(((DokScannerC *) scanner)->cpp)))
        CL_ERROR("EOF in function body");
      if (tok->type == '{') {
        ++depth;
      } else if (tok->type == '}') {
        if ((--depth) == 0) {
          cpp_unlex(((DokScannerC *) scanner)->cpp, tok);
          return;
        }
      }
    }
}



/* collect_decls:
 */
DokAST *collect_decls ( DokScanner *scanner,
                        DokAST *type,
                        DokAST *declarators )
{
  DokAST *l;
  DokAST *decl_list = NULL;
  CL_DEBUG("COLLECT DECLS:");
  CL_DEBUG("  TYPE: %s", dok_ast_to_string(type));
  CL_DEBUG("  DECLARATORS: %s", dok_ast_to_string(declarators));
  for (l = declarators; l; l = DOK_AST_LIST_NEXT(l))
    {
      DokAST *item = DOK_AST_LIST_ITEM(l);
      DokAST *item_type, *ident, *decl;
      CL_DEBUG("    - %s", dok_ast_to_string(item));
      item_type = dok_ast_declarator_chain_type(item, type);
      ident = DOK_AST_DECLARATOR_IDENT(item);
      decl = dok_ast_var_decl_new(item_type, ident);
      decl_list = dok_ast_list_append(decl_list, decl);
    }
  return decl_list;
}



/* collect_typedef:
 */
DokAST *collect_typedef ( DokScanner *scanner,
                          DokAST *type,
                          DokAST *declarator )
{
  CL_DEBUG("COLLECT TYPEDEF:");
  CL_DEBUG("  TYPE: %s", dok_ast_to_string(type));
  CL_DEBUG("  DECLARATOR: %s", dok_ast_to_string(declarator));
  CL_DEBUG("[TODO]");
  return NULL;
}



/* fix_type:
 */
DokAST *fix_type ( DokAST *type,
                   DokAST *specs )
{
  DokAST *l;
  ASSERT(DOK_AST_IS_TYPE(type));
  ASSERT((!specs) || DOK_AST_IS_LIST(specs));
  CL_DEBUG("FIX_TYPE: %s", dok_ast_to_string(type));
  CL_DEBUG("SPECS: %s", dok_ast_to_string(specs));
  for (l = specs; l; l = DOK_AST_LIST_NEXT(l))
    {
      DokAST *item = DOK_AST_LIST_ITEM(l);
      CL_DEBUG(" - %s", dok_ast_to_string(item));
    }
  CL_DEBUG("[TODO]");
  return type;
}



/* add_decls:
 */
void add_decls ( DokScanner *scanner,
                 DokAST *decls )
{
  dok_ast_unit_add_decls(((DokScannerC *) scanner)->unit, decls);
}
