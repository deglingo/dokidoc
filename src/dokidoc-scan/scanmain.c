/* scanmain.c -
 */

#include "libdokidoc/libdokidoc.h"
#include "dokidoc-base/dokidoc-sysconf.h"

#include <clog.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>



static GOptionEntry OPTION_ENTRIES[] =
  {
    { NULL, },
  };



/* get_option_context:
 */
static GOptionContext *get_option_context ( void )
{
  GOptionContext *ctxt;
  ctxt = g_option_context_new("CONFIG");
  g_option_context_add_main_entries(ctxt, OPTION_ENTRIES, NULL);
  return ctxt;
}



/* _process_scan:
 */
static DokAST *_process_scan ( DokConfig *config,
                               DokScannerClass *cls,
                               DokSourceFile *file )
{
  DokScanner *scanner;
  DokAST *ast;
  GError *error = NULL;
  CL_DEBUG("scanning file: '%s'", file->fullpath);
  scanner = dok_scanner_new(cls);
  if (!(ast = dok_scanner_process(scanner, file->fullpath, &error)))
    CL_ERROR("process error: %s", error->message);
  /* debug dump */
  {
    DokVisitor *dumper = dok_ast_dumper_new(stderr);
    CL_DEBUG("AST DUMP:");
    dok_visitor_visit(dumper, ast);
  }
  return ast;
}



/* _process_file:
 */
static void _process_file ( DokConfig *config,
                            DokScannerClass *cls,
                            DokSourceFile *file )
{
  DokAST *ast;
  DokTree *tree;
  DokVisitor *visitor;
  CL_DEBUG("pocessing file: '%s' (%s)", file->basepath, file->fullpath);
  ast = _process_scan(config, cls, file);
  /* convert AST -> DokTree */
  CL_DEBUG("AST PROCESS...");
  tree = dok_tree_root_new();
  dok_tree_file_new(tree, file->fullpath);
  visitor = dok_ast_processor_new(tree);
  dok_visitor_visit(visitor, ast);
  {
    DokVisitor *tree_dumper = dok_tree_dumper_new(stderr);
    CL_DEBUG("TREE DUMP:");
    dok_visitor_visit(tree_dumper, tree);
  }
  {
    xmlNodePtr xmlnode;
    xmlDocPtr xmldoc;
    FILE *f;
    DokVisitor *xmldumper = dok_tree_xml_dumper_new();
    dok_visitor_visit(xmldumper, tree);
    xmlnode = dok_tree_xml_dumper_get_root(xmldumper);
    CL_DEBUG("xmlnode: %p", xmlnode);
    xmldoc = xmlNewDoc(BAD_CAST "1.0");
    xmlDocSetRootElement(xmldoc, xmlnode);
    /* create xmldir */
    if (!g_file_test(config->xmldir, G_FILE_TEST_IS_DIR)) {
      if (mkdir(config->xmldir, 0777) != 0)
        CL_ERROR("could not create xmldir '%s' : %s", config->xmldir, strerror(errno));
    }
    if (!(f = fopen(file->xmlpath, "w")))
      CL_ERROR("could not open file: '%s'", file->xmlpath);
    CL_DEBUG("XML DUMP: '%s'", file->xmlpath);
    xmlDocFormatDump(f, xmldoc, 1);
    fclose(f);
  }
  /* xmldoc = xmlNewDoc(BAD_CAST "1.0"); */
  /* xmlDocSetRootElement(xmldoc, xmlnode); */
  /* CL_DEBUG("[TODO] xmldoc: %p", xmldoc); */
}



/* main:
 */
int main ( gint argc,
           gchar **argv )
{
  GOptionContext *option_context;
  GError *error;
  DokConfig *config;
  GList *l;
  CL_DEBUG("hello!");
  libdokidoc_init();
  /* parse the command line */
  option_context = get_option_context();
  if (!g_option_context_parse(option_context, &argc, &argv, &error))
    CL_ERROR("option error");
  if (argc < 2)
    CL_ERROR("CONFIG required");
  else if (argc > 2)
    CL_ERROR("too many args");
  config = dok_config_load(argv[1]);
  /* process */
  {
    gchar *modpath;
    { /* [FIXME] */
      const gchar *moddir;
      if (!(moddir = g_getenv("DOKIDOC_SCANNERSDIR")))
        moddir = DOKIDOC_SCANNERSDIR;
      modpath = g_build_filename(moddir, "dokidoc-scanner-c", NULL);
    }
    DokScannerClass *cls = dok_scanner_class_new("C", modpath);
    for (l = config->source_files; l; l = l->next)
      _process_file(config, cls, l->data);
  }

  return 0;
}
