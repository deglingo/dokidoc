/* dokscanner.h -
 */

#ifndef _DOKSCANNER_H_
#define _DOKSCANNER_H_

#include "libdokidoc/dokbase.h"



typedef struct _DokScanner DokScanner;
typedef struct _DokScannerClass DokScannerClass;
typedef struct _DokScannerFuncs DokScannerFuncs;

typedef void (* DokScannerModuleInitFunc) ( DokScannerFuncs *funcs );
typedef DokScanner * (* DokScannerNewFunc) ( void );
typedef void (* DokScannerProcessFunc) ( DokScanner *scanner,
                                         const gchar *filename,
                                         GError **error );



/* DokScanner:
 */
struct _DokScanner
{
  DokScannerClass *cls;
};



/* DokScannerFuncs:
 */
struct _DokScannerFuncs
{
  DokScannerNewFunc scanner_new;
  DokScannerProcessFunc scanner_process;
};



DokScannerClass *dok_scanner_class_new ( const gchar *name,
                                         const gchar *module_path );
DokScanner *dok_scanner_new ( DokScannerClass *cls );
gboolean dok_scanner_process ( DokScanner *scanner,
                               const gchar *filename,
                               GError **error );



#endif
