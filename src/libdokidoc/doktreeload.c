/* doktreeload.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktreeload.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>



/* Loader:
 */
typedef struct _Loader
{
  GSList *element_stack;
  GString *text;
  DokTree *tree;
  GHashTable *idmap;
  DokTree *decl;
}
  Loader;



typedef void (* StartElementFunc) ( Loader *loader,
                                    const gchar *element_name,
                                    const gchar **atnames,
                                    const gchar **atvalues,
                                    GError **error );

typedef void (* EndElementFunc) ( Loader *loader,
                                  const gchar *element_name,
                                  GError **error );

typedef void (* TextFunc) ( Loader *loader,
                            const gchar *text,
                            gssize len,
                            GError **error );



typedef struct _ElementFuncs
{
  gchar *name;
  StartElementFunc start_element;
  EndElementFunc end_element;
  TextFunc text;
}
  ElementFuncs;



static void start_root ( Loader *loader,
                         const gchar *element_name,
                         const gchar **atnames,
                         const gchar **atvalues,
                         GError **error );
static void end_root ( Loader *loader,
                       const gchar *element_name,
                       GError **error );
static void start_decl ( Loader *loader,
                         const gchar *element_name,
                         const gchar **atnames,
                         const gchar **atvalues,
                         GError **error );
static void end_decl ( Loader *loader,
                       const gchar *element_name,
                       GError **error );

static ElementFuncs ELEMENT_FUNCS[] =
  {
    { "root", start_root, end_root, NULL, },
    { "decl", start_decl, end_decl, NULL, },
    { NULL, },
  };



static inline ElementFuncs *element_funcs_lookup ( const gchar *name )
{
  ElementFuncs *f;
  for (f = ELEMENT_FUNCS; f->name; f++)
    {
      if (!strcmp(f->name, name))
        return f;
    }
  return NULL;
}



static void on_start_element ( GMarkupParseContext *context,
                               const gchar *element_name,
                               const gchar **atnames,
                               const gchar **atvalues,
                               gpointer data,
                               GError **error );
static void on_end_element ( GMarkupParseContext *context,
                             const gchar *element_name,
                             gpointer data,
                             GError **error );
static void on_text ( GMarkupParseContext *context,
                      const gchar *text,
                      gsize text_len,
                      gpointer data,
                      GError **error );
static void on_passthrough ( GMarkupParseContext *context,
                             const gchar *text,
                             gsize text_len,
                             gpointer data,
                             GError **error );
static void on_error ( GMarkupParseContext *context,
                       GError *error,
                       gpointer data );



static GMarkupParser MARKUP_PARSER =
  {
    on_start_element,
    on_end_element,
    on_text,
    on_passthrough,
    on_error,
  };



static const gchar *getattr ( const gchar *name,
                              const gchar **atnames,
                              const gchar **atvalues )
{
  guint i;
  for (i = 0; atnames[i]; i++)
    {
      if (!strcmp(atnames[i], name))
        return atvalues[i];
    }
  return NULL;
}



static const gchar *reqattr ( const gchar *name,
                              const gchar **atnames,
                              const gchar **atvalues,
                              GError **error )
{
  const gchar *value;
  if (!(value = getattr(name, atnames, atvalues)))
    CL_ERROR("[TODO] attribute required: '%s'", name);
  return value;
}



/* dok_tree_load:
 */
DokTree *dok_tree_load ( const gchar *fname )
{
  GMarkupParseContext *context;
  FILE *f;
  gchar buffer[1024];
  gint fno;
  GError *error = NULL;
  Loader *loader;

  CL_DEBUG("loading doktree file: '%s'", fname);

  loader = g_new0(Loader, 1);
  loader->text = g_string_new("");
  loader->idmap = g_hash_table_new(NULL, NULL);
  context = g_markup_parse_context_new(&MARKUP_PARSER, 0, loader, NULL);
  if (!(f = fopen(fname, "r")))
    CL_ERROR("could not open file '%s' : %s", fname, strerror(errno));
  fno = fileno(f);
  while (1)
    {
      ssize_t r;
      r = read(fno, buffer, 1024);
      if (r > 0)
        {
          if (!g_markup_parse_context_parse(context, buffer, r, &error))
            CL_ERROR("parse error: %s", error->message);
        }
      else if (r == 0)
        {
          if (!g_markup_parse_context_end_parse(context, &error))
            CL_ERROR("parse error: %s", error->message);
          break;
        }
      else
        {
          CL_ERROR("read error: %s", strerror(errno));
        }
    }
  fclose(f);

  ASSERT(loader->tree);
  return loader->tree;
}



/* on_start_element:
 */
static void on_start_element ( GMarkupParseContext *context,
                               const gchar *element_name,
                               const gchar **atnames,
                               const gchar **atvalues,
                               gpointer data,
                               GError **error )
{
  Loader *loader = data;
  ElementFuncs *funcs;
  CL_TRACE("%p", context);
  if (!(funcs = element_funcs_lookup(element_name)))
    CL_ERROR("unknown element: '%s'", element_name);
  loader->element_stack = g_slist_prepend(loader->element_stack, funcs);
  if (funcs->start_element)
    funcs->start_element(loader, element_name, atnames, atvalues, error);
}



/* on_end_element:
 */
static void on_end_element ( GMarkupParseContext *context,
                             const gchar *element_name,
                             gpointer data,
                             GError **error )
{
  Loader *loader = data;
  ElementFuncs *funcs;
  gchar *text;
  CL_TRACE("%p", context);
  ASSERT(loader->element_stack);
  funcs = loader->element_stack->data;
  loader->element_stack = g_slist_delete_link(loader->element_stack, loader->element_stack);
  /* text */
  text = g_strstrip(loader->text->str);
  if (text[0]) {
    if (funcs->text) {
      funcs->text(loader, text, -1 /* [FIXME] */, error);
    } else {
      CL_DEBUG("[TODO] no text handler for '%s'", element_name);
    }
  }  
  g_string_truncate(loader->text, 0);
  /* end-element */
  ASSERT(!strcmp(element_name, funcs->name));
  if (funcs->end_element)
    funcs->end_element(loader, element_name, error);
}



/* on_text:
 */
static void on_text ( GMarkupParseContext *context,
                      const gchar *text,
                      gsize text_len,
                      gpointer data,
                      GError **error )
{
  Loader *loader = data;
  /* CL_TRACE("%p", context); */
  g_string_append_len(loader->text, text, text_len);
}



/* on_passthrough:
 */
static void on_passthrough ( GMarkupParseContext *context,
                             const gchar *text,
                             gsize text_len,
                             gpointer data,
                             GError **error )
{
  CL_TRACE("%p", context);
}



/* on_error:
 */
static void on_error ( GMarkupParseContext *context,
                       GError *error,
                       gpointer data )
{
  CL_TRACE("%p", context);
}



/* start_root:
 */
static void start_root ( Loader *loader,
                         const gchar *element_name,
                         const gchar **atnames,
                         const gchar **atvalues,
                         GError **error )
{
  CL_TRACE("%s", element_name);
  loader->tree = dok_tree_root_new();
}



/* end_root:
 */
static void end_root ( Loader *loader,
                       const gchar *element_name,
                       GError **error )
{
  CL_TRACE("%s", element_name);
}



/* start_decl:
 */
static void start_decl ( Loader *loader,
                         const gchar *element_name,
                         const gchar **atnames,
                         const gchar **atvalues,
                         GError **error )
{
  const gchar *name = reqattr("name", atnames, atvalues, error);
  const gchar *stype = reqattr("type", atnames, atvalues, error);
  const gchar *sid = reqattr("id", atnames, atvalues, error);
  const gchar *scontext = reqattr("context", atnames, atvalues, error);
  gint type, id, context;
  gchar *end;
  type = strtol(stype, &end, 10);
  ASSERT(!(*end));
  id = strtol(sid, &end, 10);
  ASSERT(!(*end));
  context = strtol(scontext, &end, 10);
  ASSERT(!(*end));
  CL_TRACE("%s, name=%s, type=%d, id=%d, context=%d", element_name, name, type, id, context);
  ASSERT(!loader->decl);
  switch (type)
    {
    case DOK_TREE_DECL_NAMESPACE:
      ASSERT(context == 0);
      loader->decl = dok_tree_get_namespace(loader->tree, NULL, name);
      g_hash_table_insert(loader->idmap, 
                          GUINT_TO_POINTER(id),
                          GUINT_TO_POINTER(DOK_TREE_DECL(loader->decl)->id));
      break;
    case DOK_TREE_DECL_VAR:
      {
        DokTreeID real_id = GPOINTER_TO_UINT(g_hash_table_lookup(loader->idmap,
                                                                 GUINT_TO_POINTER(context)));
        DokTree *real_context = dok_tree_get_decl_by_id(loader->tree, real_id);
        loader->decl = dok_tree_get_var(loader->tree, real_context, name);
        g_hash_table_insert(loader->idmap, 
                            GUINT_TO_POINTER(id),
                            GUINT_TO_POINTER(DOK_TREE_DECL(loader->decl)->id));
      }
      break;
    default:
      CL_DEBUG("[TODO] decl type %d", type);
    }
}



/* end_decl:
 */
static void end_decl ( Loader *loader,
                       const gchar *element_name,
                       GError **error )
{
  CL_TRACE("%s", element_name);
  loader->decl = NULL;
}
