/* scanmain.c -
 */

#include "libdokidoc/libdokidoc.h"
#include "dokidoc-base/dokidoc-sysconf.h"

#include <clog.h>
#include <glib.h>
#include <glob.h>
#include <string.h>
#include <stdlib.h>



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
  gchar *fullpath;
  gchar *basepath;
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
      if (!strcmp(path, ((SourceFile *) (l->data))->fullpath))
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
  return strcmp(((SourceFile *) a)->fullpath,
                ((SourceFile *) b)->fullpath);
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
      source_file->fullpath = g_strdup(full_path);
      source_file->basepath = source_file->fullpath + strlen(source_file->base->path) + 1;
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
                            DokScannerClass *cls,
                            SourceFile *file )
{
  DokScanner *scanner;
  DokVisitor *visitor;
  DokAST *ast;
  GError *error = NULL;
  gchar *xmlfile;
  CL_DEBUG("pocessing file: '%s' (%s)", file->basepath, file->fullpath);
  scanner = dok_scanner_new(cls);
  if (!(ast = dok_scanner_process(scanner, file->fullpath, &error)))
    CL_ERROR("process error: %s", error->message);
  /* dump */
  {
    DokVisitor *dumper = dok_ast_dumper_new(stderr);
    CL_DEBUG("AST DUMP:");
    dok_visitor_visit(dumper, ast);
  }
  /* process */
  CL_DEBUG("AST PROCESS...");
  visitor = dok_ast_processor_new();
  dok_visitor_visit(visitor, ast);
  {
    DokTree *tree = dok_ast_processor_get_tree(visitor);
    DokVisitor *tree_dumper = dok_tree_dumper_new(stderr);
    CL_DEBUG("TREE DUMP:");
    dok_visitor_visit(tree_dumper, tree);
  }
  {
    xmlNodePtr xmlnode;
    xmlDocPtr xmldoc;
    gchar *c, *x;
    FILE *f;
    xmlfile = g_malloc(strlen(file->basepath)+5);
    for (c = file->basepath, x = xmlfile; *c; c++, x++)
      *x = ((*c) == '/') ? '$' : (*c);
    sprintf(x, ".xml");
    DokTree *tree = dok_ast_processor_get_tree(visitor);
    DokVisitor *xmldumper = dok_tree_xml_dumper_new();
    dok_visitor_visit(xmldumper, tree);
    xmlnode = dok_tree_xml_dumper_get_root(xmldumper);
    CL_DEBUG("xmlnode: %p", xmlnode);
    xmldoc = xmlNewDoc(BAD_CAST "1.0");
    xmlDocSetRootElement(xmldoc, xmlnode);
    if (!(f = fopen(xmlfile, "w")))
      CL_ERROR("could not open file: '%s'", xmlfile);
    CL_DEBUG("XML DUMP: '%s'", xmlfile);
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
  Config *config;
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
  config = g_new0(Config, 1);
  parse_config(config, argv[1]);
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
