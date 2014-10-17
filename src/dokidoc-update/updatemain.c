/* filtmain.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/libdokidoc.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>



/* popen_filter:
 */
static GPid popen_filter ( DokConfig *config,
                           gchar *tmplpath,
                           gint *stdout,
                           GError **error )
{
  gboolean r;
  GPid pid;
  gchar *argv[] =
    {
      NULL,
      NULL,
      "read",
      NULL,
    };
  argv[0] = config->filter;
  argv[1] = tmplpath;
  CL_DEBUG("popen_filter: '%s' '%s'", config->filter, tmplpath);
  r = g_spawn_async_with_pipes
    (NULL,
     argv,
     NULL,
     G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD,
     NULL,
     NULL,
     &pid,
     NULL,
     stdout,
     NULL,
     error);
  if (r)
    return pid;
  else
    return 0;
}



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
  /* read the templates */
  for (l = config->templates; l; l = l->next)
    {
      gchar buffer[1025];
      GPid filter_pid;
      gint filter_fdout;
      GError *error = NULL;
      pid_t rwait;
      gint rstatus;
      DokTmplFile *tmpl = l->data;
      if ((filter_pid = popen_filter(config, tmpl->tmplpath, &filter_fdout, &error)) == 0)
        CL_ERROR("could not open filter process: %s", error->message);
      /* read the data */
      CL_DEBUG("filter output:");
      while (1)
        {
          ssize_t r = read(filter_fdout, buffer, 1024);
          if (r > 0)
            {
              buffer[r] = 0;
              fprintf(stderr, "%s", buffer);
            }
          else if (r == 0)
            {
              break;
            }
          else
            {
              CL_ERROR("read error");
            }
        }
      /* wait filter proc */
      rwait = waitpid(filter_pid, &rstatus, 0);
      if (rwait <= 0)
        CL_ERROR("waitpid failed: %s", strerror(errno));
      if (rstatus != 0)
        CL_ERROR("filter failed");
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
