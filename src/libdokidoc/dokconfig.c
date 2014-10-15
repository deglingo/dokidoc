/* dokconfig.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokconfig.h"

#include <glob.h>
#include <string.h>
#include <stdlib.h>



/* _get_source_bases:
 */
static void _get_source_bases ( DokConfig *config,
                                GKeyFile *kfile )
{
  gchar **bases;
  gsize n_bases, n;
  GError *error = NULL;
  if (!(bases = g_key_file_get_string_list(kfile, "DEFAULT", "source_bases", &n_bases, &error)))
    CL_ERROR("source_bases required");
  /* CL_DEBUG("%d source bases:", n_bases); */
  for (n = 0; n < n_bases; n++)
    {
      gchar *path = realpath(g_strstrip(bases[n]), NULL);
      DokSourceBase *source_base = g_new0(DokSourceBase, 1);
      source_base->path = g_strdup(path);
      /* CL_DEBUG(" - '%s'", source_base->path); */
      free(path);
      config->source_bases = g_list_append(config->source_bases, source_base);
    }
  g_strfreev(bases);
}



/* _find_file:
 */
static DokSourceFile *_find_file ( DokConfig *config,
                                const gchar *path )
{
  GList *l;
  for (l = config->source_files; l; l = l->next)
    {
      if (!strcmp(path, ((DokSourceFile *) (l->data))->fullpath))
        return l->data;
    }
  return NULL;
}



/* _find_base:
 */
static DokSourceBase *_find_base ( DokConfig *config,
                                const gchar *path )
{
  GList *l;
  for (l = config->source_bases; l; l = l->next)
    {
      DokSourceBase *base = l->data;
      gsize len = strlen(base->path);
      if (strlen(path) > len &&
          (!strncmp(path, base->path, len)) &&
          path[len] == '/')
        return base;
    }
  return NULL;
}



/* _sort_source_files:
 */
static gint _sort_source_files ( gconstpointer a,
                                 gconstpointer b )
{
  return strcmp(((DokSourceFile *) a)->fullpath,
                ((DokSourceFile *) b)->fullpath);
}



/* _get_xmlpath:
 */
static gchar *_get_xmlpath ( DokConfig *config,
                             DokSourceFile *file )
{
  gchar *base = g_alloca(strlen(file->basepath) + 5);
  gchar *path;
  gchar *c, *x;
  for (c = file->basepath, x = base; *c; c++, x++)
    *x = ((*c) == '/') ? '$' : (*c);
  sprintf(x, ".xml");
  path = g_build_filename(config->xmldir, base, NULL);
  return path;
}



/* _get_sources:
 */
static void _get_sources ( DokConfig *config,
                           GKeyFile *kfile )
{
  gchar **sources;
  gsize n_sources, n;
  glob_t vector = { 0, };
  GError *error = NULL;
  if (!(sources = g_key_file_get_string_list(kfile, "DEFAULT", "sources", &n_sources, &error)))
    CL_ERROR("sources required");
  /* CL_DEBUG("found %d sources:", n_sources); */
  /* glob patterns */
  for (n = 0; n < n_sources; n++)
    {
      gint r;
      gchar *pattern = g_strstrip(sources[n]);
      /* CL_DEBUG(" - '%s'", pattern); */
      if ((r = glob(pattern, GLOB_APPEND | GLOB_NOSORT, NULL, &vector)) != 0)
        CL_ERROR("glob failed: %d", r);
    }
  /* create files */
  for (n = 0; n < vector.gl_pathc; n++)
    {
      gchar *full_path = realpath(vector.gl_pathv[n], NULL);
      DokSourceFile *source_file;
      if (_find_file(config, full_path))
        continue;
      source_file = g_new0(DokSourceFile, 1);
      if (!(source_file->base = _find_base(config, full_path)))
        CL_ERROR("no source base found for '%s'", full_path);
      source_file->fullpath = g_strdup(full_path);
      source_file->basepath = source_file->fullpath + strlen(source_file->base->path) + 1;
      source_file->xmlpath = _get_xmlpath(config, source_file);
      config->source_files = g_list_append(config->source_files, source_file);
      free(full_path);
    }
  config->source_files = g_list_sort(config->source_files, _sort_source_files);
  /* { */
  /*   GList *l; */
  /*   CL_DEBUG("found %d files:", g_list_length(config->source_files)); */
  /*   for (l = config->source_files; l; l = l->next) */
  /*     CL_DEBUG(" - '%s'", ((DokSourceFile *) (l->data))->path); */
  /* } */
}



/* parse_config:
 */
static void parse_config ( DokConfig *config,
                           const gchar *cfgfile )
{
  GKeyFile *kfile;
  GError *error = NULL;
  CL_DEBUG("reading config file: '%s'", cfgfile);
  kfile = g_key_file_new();
  g_key_file_set_list_separator(kfile, ',');
  if (!g_key_file_load_from_file(kfile, cfgfile, 0, &error))
    CL_ERROR("error in config file: %s", error->message);
  /* get params */
  if (!(config->xmldir = g_key_file_get_string(kfile, "DEFAULT", "xmldir", &error))) {
    config->xmldir = g_strdup("xml");
  }
  _get_source_bases(config, kfile);
  _get_sources(config, kfile);
}



/* dok_config_load:
 */
DokConfig *dok_config_load ( const gchar *fname )
{
  DokConfig *config = g_new0(DokConfig, 1);
  parse_config(config, fname);
  return config;
}
