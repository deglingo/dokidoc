/* dokscanner.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokscanner.h"
#include <gmodule.h>



/* DokScannerClass:
 */
struct _DokScannerClass
{
  gchar *name;
  gchar *module_path;
  GModule *module;
  DokScannerFuncs funcs;
};



/* dok_scanner_class_new:
 */
DokScannerClass *dok_scanner_class_new ( const gchar *name,
                                         const gchar *module_path )
{
  DokScannerClass *cls;
  cls = g_new0(DokScannerClass, 1);
  cls->name = g_strdup(name);
  cls->module_path = g_strdup(module_path);
  return cls;
}



/* dok_scanner_class_init:
 */
static void dok_scanner_class_init ( DokScannerClass *cls )
{
  if (!cls->module)
    {
      gpointer initptr;
      DokScannerModuleInitFunc init;
      if (!(cls->module = g_module_open(cls->module_path, G_MODULE_BIND_LOCAL)))
        CL_ERROR("could not load module '%s' : %s", cls->module_path, g_module_error());
      if (!g_module_symbol(cls->module, "dokidoc_scanner_module_init", &initptr))
        CL_ERROR("could not get symbol 'dokidoc_scanner_module_init'");
      init = initptr;
      init(&cls->funcs);
    }
}



/* dok_scanner_new:
 */
DokScanner *dok_scanner_new ( DokScannerClass *cls )
{
  DokScanner *scanner;
  dok_scanner_class_init(cls);
  scanner = cls->funcs.scanner_new();
  scanner->cls = cls;
  return scanner;
}



/* dok_scanner_process:
 */
DokAST *dok_scanner_process ( DokScanner *scanner,
                              const gchar *filename,
                              GError **error )
{
  GError *tmperr = NULL;
  DokAST *ast;
  ast = scanner->cls->funcs.scanner_process(scanner, filename, &tmperr);
  if (ast)
    {
      ASSERT(!tmperr);
      return ast;
    }
  else
    {
      ASSERT(tmperr);
      g_propagate_error(error, tmperr);
      return NULL;
    }
}
