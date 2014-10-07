/* scanmain.c -
 */

#include "libdokidoc/libdokidoc.h"
#include "dokidoc-base/dokidoc-sysconf.h"

#include <clog.h>
#include <glib.h>
#include <gmodule.h>
#include <glob.h>
#include <string.h>
#include <stdlib.h>



typedef struct _ScannerClass ScannerClass;
typedef struct _Scanner Scanner;

typedef void (* ProcessFunc) ( Scanner *scanner,
                               GError **error );



struct _ScannerClass
{
  gchar *name;
  gchar *module_path;
  GModule *module;
  ProcessFunc process;
};



struct _Scanner
{
  ScannerClass *cls;
  gchar *filename;
};



typedef struct _SourceBase
{
  /* realpath + one trailing slash to make file matching easier */
  gchar *path;
}
  SourceBase;



typedef struct _SourceFile
{
  SourceBase *base;
  /* realpath */
  gchar *path;
}
  SourceFile;



typedef struct _Config
{
  GList *source_bases;
  GList *source_files;
}
  Config;



static GOptionEntry OPTION_ENTRIES[] =
  {
    { NULL, },
  };



/* scanner_class_new:
 */
static ScannerClass *scanner_class_new ( const gchar *name,
                                         const gchar *module_path )
{
  ScannerClass *cls;
  cls = g_new0(ScannerClass, 1);
  cls->name = g_strdup(name);
  cls->module_path = g_strdup(module_path);
  return cls;
}



/* scanner_new:
 */
static Scanner *scanner_new ( ScannerClass *cls,
                              gchar *filename )
{
  Scanner *scanner;
  if (!cls->module)
    {
      gpointer process;
      CL_DEBUG("opening module: '%s'", cls->module_path);
      if (!(cls->module = g_module_open(cls->module_path, G_MODULE_BIND_LOCAL)))
        CL_ERROR("could not load module!");
      if (!(g_module_symbol(cls->module, "dokidoc_scanner_process", &process)))
        CL_ERROR("could not find 'dokidoc_scanner_process' symbol in module");
      cls->process = process;
    }
  scanner = g_new0(Scanner, 1);
  scanner->cls = cls;
  scanner->filename = g_strdup(filename);
  return scanner;
}



/* scanner_process:
 */
static void scanner_process ( Scanner *scanner )
{
  GError *error = NULL;
  scanner->cls->process(scanner, &error);
  if (error)
    CL_ERROR("process error: %s", error->message);
}



/* get_option_context:
 */
static GOptionContext *get_option_context ( void )
{
  GOptionContext *ctxt;
  ctxt = g_option_context_new("CONFIG");
  g_option_context_add_main_entries(ctxt, OPTION_ENTRIES, NULL);
  return ctxt;
}



/* _get_source_bases:
 */
static void _get_source_bases ( Config *config,
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
      SourceBase *source_base = g_new0(SourceBase, 1);
      source_base->path = g_strdup(path);
      /* CL_DEBUG(" - '%s'", source_base->path); */
      free(path);
      config->source_bases = g_list_append(config->source_bases, source_base);
    }
  g_strfreev(bases);
}



/* _find_file:
 */
static SourceFile *_find_file ( Config *config,
                                const gchar *path )
{
  GList *l;
  for (l = config->source_files; l; l = l->next)
    {
      if (!strcmp(path, ((SourceFile *) (l->data))->path))
        return l->data;
    }
  return NULL;
}



/* _find_base:
 */
static SourceBase *_find_base ( Config *config,
                                const gchar *path )
{
  GList *l;
  for (l = config->source_bases; l; l = l->next)
    {
      SourceBase *base = l->data;
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
  return strcmp(((SourceFile *) a)->path,
                ((SourceFile *) b)->path);
}



/* _get_sources:
 */
static void _get_sources ( Config *config,
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
      SourceFile *source_file;
      if (_find_file(config, full_path))
        continue;
      source_file = g_new0(SourceFile, 1);
      if (!(source_file->base = _find_base(config, full_path)))
        CL_ERROR("no source base found for '%s'", full_path);
      source_file->path = g_strdup(full_path);
      config->source_files = g_list_append(config->source_files, source_file);
      free(full_path);
    }
  config->source_files = g_list_sort(config->source_files, _sort_source_files);
  /* { */
  /*   GList *l; */
  /*   CL_DEBUG("found %d files:", g_list_length(config->source_files)); */
  /*   for (l = config->source_files; l; l = l->next) */
  /*     CL_DEBUG(" - '%s'", ((SourceFile *) (l->data))->path); */
  /* } */
}



/* parse_config:
 */
static void parse_config ( Config *config,
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
  _get_source_bases(config, kfile);
  _get_sources(config, kfile);
}



/* _process_file:
 */
static void _process_file ( Config *config,
                            ScannerClass *cls,
                            SourceFile *file )
{
  Scanner *scanner;
  CL_DEBUG("pocessing file: '%s'", file->path);
  scanner = scanner_new(cls, file->path);
  scanner_process(scanner);
}



/* main:
 */
int main ( gint argc,
           gchar **argv )
{
  GOptionContext *option_context;
  GError *error;
  Config *config;
  GList *l;
  CL_DEBUG("hello!");
  /* parse the command line */
  option_context = get_option_context();
  if (!g_option_context_parse(option_context, &argc, &argv, &error))
    CL_ERROR("option error");
  if (argc < 2)
    CL_ERROR("CONFIG required");
  else if (argc > 2)
    CL_ERROR("too many args");
  config = g_new0(Config, 1);
  parse_config(config, argv[1]);
  /* process */
  {
    ScannerClass *cls = scanner_class_new("C", g_build_filename(DOKIDOC_SCANNERSDIR, "dokidoc-scanner-c.so", NULL));
    for (l = config->source_files; l; l = l->next)
      _process_file(config, cls, l->data);
  }
  return 0;
}
