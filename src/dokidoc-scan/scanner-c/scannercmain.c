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
  yyparse(scanner);
}



/* dokidoc_scanner_module_init:
 */
void dokidoc_scanner_module_init ( DokScannerFuncs *funcs )
{
  CL_DEBUG("SCAN PROCESS...");
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
  cpp_lex(((DokScannerC *) scanner)->cpp);
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
