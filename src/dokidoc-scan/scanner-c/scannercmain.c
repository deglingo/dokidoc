/* scannercmain.c -
 */

#include "libdokidoc/libdokidoc.h"
#include "scannerc.h"
#include "scannercgram.h"
#include <glib.h>
#include <clog.h>



/* LexRule:
 */
typedef struct _LexRule
{
  gint token;
  gchar *pattern;
  GRegex *regex;
}
  LexRule;



/* DokScannerC:
 */
typedef struct _DokScannerC
{
  DokScanner dok_scanner;
  GMappedFile *mfile;
  gsize mfile_len;
  gchar *mfile_buf;
  gsize mfile_pos;
}
  DokScannerC;



/* LEX_RULES:
 */
static LexRule LEX_RULES[] =
  {
    { TOK_IDENT, "^[a-zA-Z_][a-zA-Z0-9_]*", },
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
  if (!(cscanner->mfile = g_mapped_file_new(filename, FALSE, error)))
    CL_ERROR("could not open file: %s", (*error)->message);
  cscanner->mfile_len = g_mapped_file_get_length(cscanner->mfile);
  cscanner->mfile_buf = g_mapped_file_get_contents(cscanner->mfile);
  CL_DEBUG("process: '%s'", filename);
  yyparse(scanner);
}



/* dokidoc_scanner_module_init:
 */
void dokidoc_scanner_module_init ( DokScannerFuncs *funcs )
{
  LexRule *rule;
  CL_DEBUG("SCAN PROCESS...");
  for (rule = LEX_RULES; rule->token; rule++)
    {
      GError *err = NULL;
      if (!(rule->regex = g_regex_new(rule->pattern, G_REGEX_OPTIMIZE, 0, &err)))
        CL_ERROR("regex error: %s", err->message);
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
  /* LexRule *rule; */
  /* for (rule = LEX_RULES;  */
  CL_TRACE("%p", scanner);
  return 0;
}



/* yyerror:
 */
void yyerror ( Loc *locp,
               DokScanner *scanner,
               char const *msg )
{
  CL_ERROR("parse error: %s", msg);
}

