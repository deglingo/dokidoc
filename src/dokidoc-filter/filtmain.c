/* filtmain.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/libdokidoc.h"
#include <string.h>



enum
  {
    CMD_INPUT,
    CMD_OUTPUT,
  };



/* dump_templates:
 */
static void dump_templates ( DokConfig *config,
                             DokTree *tree )
{
  DokVisitor *visitor;
  CL_DEBUG("TEMPLATES DUMP");
  visitor = dok_tree_tmpl_dumper_new();
  dok_visitor_visit(visitor, tree);
}



/* proc_input:
 */
static void proc_input ( DokConfig *config )
{
  GList *l;
  DokTree *tree = dok_tree_root_new();
  for (l = config->source_files; l; l = l->next)
    {
      DokSourceFile *file = l->data;
      if (!dok_tree_load(tree, file->xmlpath))
        CL_ERROR("could not load doktree: '%s'", file->xmlpath);
    }
  /* [TODO] load templates */
  dump_templates(config, tree);
}



/* proc_output:
 */
static void proc_output ( DokConfig *config )
{
  CL_ERROR("[TODO]");
}



/* main:
 */
gint main ( gint argc,
            gchar **argv )
{
  gint cmd;
  gchar *cfgfile;
  DokConfig *config;
  libdokidoc_init();
  CL_DEBUG("hello!");
  /* command line */
  ASSERT(argc == 3);
  if (!strcmp(argv[1], "-i")) {
    cmd = CMD_INPUT;
  } else if (!strcmp(argv[1], "-o")) {
    cmd = CMD_OUTPUT;
  } else {
    CL_ERROR("invalid option: '%s'", argv[1]);
  }
  cfgfile = argv[2];
  config = dok_config_load(cfgfile);
  switch (cmd) {
  case CMD_INPUT:
    proc_input(config);
    break;
  case CMD_OUTPUT:
    proc_output(config);
    break;
  default:
    CL_ERROR("[TODO]");
  }
  return 0;
}
