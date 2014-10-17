/* filtmain.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/libdokidoc.h"



/* process:
 */
static void process ( DokConfig *config )
{
  DokTree *tree;
  GList *l;
  /* load xml */
  tree = dok_tree_root_new();
  for (l = config->source_files; l; l = l->next)
    {
      DokSourceFile *src = l->data;
      if (!dok_tree_load(tree, src->xmlpath))
        CL_ERROR("could not load dok tree: '%s'", src->xmlpath);
    }
}



/* main:
 */
gint main ( gint argc,
            gchar **argv )
{
  gchar *cfgfile;
  DokConfig *config;
  libdokidoc_init();
  CL_DEBUG("hello!");
  /* [TODO] command line */
  ASSERT(argc == 2);
  cfgfile = argv[1];
  config = dok_config_load(cfgfile);
  process(config);
  return 0;
}
