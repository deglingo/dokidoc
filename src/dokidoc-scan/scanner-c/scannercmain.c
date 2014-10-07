/* scannercmain.c -
 */

#include "libdokidoc/libdokidoc.h"
#include <glib.h>
#include <clog.h>



/* DokScannerC:
 */
typedef struct _DokScannerC
{
  DokScanner dok_scanner;
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
  CL_DEBUG("process: '%s'", filename);
}



/* dokidoc_scanner_module_init:
 */
void dokidoc_scanner_module_init ( DokScannerFuncs *funcs )
{
  CL_DEBUG("SCAN PROCESS...");
  funcs->scanner_new = _scanner_new;
  funcs->scanner_process = _scanner_process;
}
