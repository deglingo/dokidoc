/* filtmain.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/libdokidoc.h"

#include <string.h>
#include <errno.h>



/* process:
 */
static void process ( DokConfig *config )
{
  DokTree *tree;
  GList *l;
  DokVisitor *dumper;
  GHashTable *tmplmap = g_hash_table_new(g_str_hash, g_str_equal);
  GList *templates = NULL;
  /* load the xml tree */
  tree = dok_tree_root_new();
  for (l = config->source_files; l; l = l->next)
    {
      DokSourceFile *src = l->data;
      if (!dok_tree_load(tree, src->xmlpath))
        CL_ERROR("could not load dok tree: '%s'", src->xmlpath);
    }
  /* [TODO] read the templates */
  for (l = config->templates; l; l = l->next)
    {
      DokTmplFile *tmplfile = l->data;
      DokTemplate *tmpl = dok_template_new(tmplfile->tmplpath);
      GList *l2;
      for (l2 = tmplfile->sources; l2; l2 = l2->next)
        {
          DokSourceFile *src = l2->data;
          g_hash_table_insert(tmplmap, src->fullpath, tmpl);
        }
      templates = g_list_append(templates, tmpl);
    }
  /* create the dumper */
  dumper = dok_tree_tmpl_dumper_new(tmplmap);
  dok_visitor_visit(dumper, tree);
  /* output the templates */
  for (l = templates; l; l = l->next)
    {
      DokTemplate *tmpl = l->data;
      FILE *f;
      if (!(f = fopen(tmpl->path, "w")))
        CL_ERROR("could not open file '%s' : %s", tmpl->path, strerror(errno));
      dok_template_dump(tmpl, f);
      fclose(f);
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
