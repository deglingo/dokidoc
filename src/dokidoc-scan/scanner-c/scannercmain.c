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
  return (DokScanner *) scanner;
}



/* _scanner_process:
 */
static void _scanner_process ( DokScanner *scanner,
                               const gchar *filename,
                               GError **error )
{
  DokScannerC *cscanner = (DokScannerC *) scanner;
  cscanner->cpp = cpp_new(filename);
  if (yyparse(scanner) != 0)
    CL_ERROR("parse failed");
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
  CL_DEBUG("COLLECT DECLS:");
  CL_DEBUG("  TYPE: %s", dok_ast_to_string(type));
  CL_DEBUG("  DECLARATORS: %s", dok_ast_to_string(declarators));
  for (l = declarators; l; l = DOK_AST_LIST_NEXT(l))
    {
      DokAST *item = DOK_AST_LIST_ITEM(l);
      CL_DEBUG("    - %s", dok_ast_to_string(item));
    }
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
