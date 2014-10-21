/* doktreeload.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktreeload.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>



/* Loader:
 */
typedef struct _Loader
{
  GSList *element_stack;
  GString *text;
  DokTree *tree;
  GHashTable *idmap;
  DokTree *item;
}
  Loader;



typedef enum _CollectFlags
  {
    COLLECT_FLAG_STRING = 1,
    COLLECT_FLAG_UINT,
    COLLECT_FLAG_BOOL,

    COLLECT_FLAG_OPTIONAL = 1 << 8,
  }
  CollectFlags;

#define COLLECT_FLAGS_TYPE_MASK 0xff



static gboolean collect_attrs ( const gchar **atnames,
                                const gchar **atvalues,
                                GError **error,
                                CollectFlags first_flags,
                                const gchar *first_name,
                                ... );



static gint _find_attr ( const gchar **atnames,
                         const gchar *name )
{
  gint index;
  for (index = 0; atnames[index]; index++)
    {
      if (!strcmp(atnames[index], name))
        return index;
    }
  return -1;
}



static gboolean _collect_string ( const gchar **atnames,
                                  const gchar **atvalues,
                                  CollectFlags flags,
                                  const gchar *name,
                                  const gchar **dest,
                                  GError **error )
{
  gint index;
  if ((index = _find_attr(atnames, name)) >= 0)
    {
      *dest = atvalues[index];
      return TRUE;
    }
  else
    {
      if (flags & COLLECT_FLAG_OPTIONAL)
        {
          *dest = "";
          return TRUE;
        }
      else
        {
          CL_ERROR("[TODO] attr not found: '%s'", name);
          return FALSE;
        }
    }
}



static gboolean _collect_uint ( const gchar **atnames,
                                const gchar **atvalues,
                                CollectFlags flags,
                                const gchar *name,
                                guint *dest,
                                GError **error )
{
  const gchar *str;
  gchar *end;
  if (!_collect_string(atnames, atvalues, flags, name, &str, error))
    return FALSE;
  if (!str[0])
    {
      if (flags & COLLECT_FLAG_OPTIONAL)
        {
          *dest = 0;
          return TRUE;
        }
      else
        {
          CL_ERROR("[TODO] attr missing: '%s'", name);
          return FALSE;
        }
    }
  *dest = strtoul(str, &end, 10); /* base 0 ? */
  if (end[0])
    CL_ERROR("[TODO] invalid number: '%s'", name);
  return TRUE;
}



struct bool_spec
{
  gchar *name;
  gboolean value;
};



static gboolean _collect_bool ( const gchar **atnames,
                                const gchar **atvalues,
                                CollectFlags flags,
                                const gchar *name,
                                gboolean *dest,
                                GError **error )
{
  static struct bool_spec specs[] = {
    { "1", TRUE },
    { "0", FALSE },
    { NULL, },
  };
  struct bool_spec *bs;
  const gchar *str;
  if (!_collect_string(atnames, atvalues, flags, name, &str, error))
    return FALSE;
  for (bs = specs; bs->name; bs++)
    {
      if (!strcmp(str, bs->name))
        {
          *dest = bs->value;
          return TRUE;
        }
    }
  CL_ERROR("invalid bool value: '%s'", str);
  return FALSE;
}



static gboolean collect_attrs ( const gchar **atnames,
                                const gchar **atvalues,
                                GError **error,
                                CollectFlags first_flags,
                                const gchar *first_name,
                                ... )
{
  GError *tmperr = NULL;
  CollectFlags flags;
  const gchar *name;
  va_list args;
  va_start(args, first_name);
  flags = first_flags;
  name = first_name;
  while (1)
    {
      CollectFlags flags_type = flags & COLLECT_FLAGS_TYPE_MASK;
      switch (flags_type) {
      case COLLECT_FLAG_STRING:
        {
          const gchar **dest = va_arg(args, const gchar **);
          if (!_collect_string(atnames, atvalues, flags, name, dest, &tmperr))
            goto end;
        }
        break;
      case COLLECT_FLAG_UINT:
        {
          guint *dest = va_arg(args, guint *);
          if (!_collect_uint(atnames, atvalues, flags, name, dest, &tmperr))
            goto end;
        }
        break;
      case COLLECT_FLAG_BOOL:
        {
          gboolean *dest = va_arg(args, gboolean *);
          if (!_collect_bool(atnames, atvalues, flags, name, dest, &tmperr))
            goto end;
        }
        break;
      default:
        CL_ERROR("[TODO] type %d", flags_type);
      }
      if ((flags = va_arg(args, CollectFlags)) == 0)
        break;
      name = va_arg(args, const gchar *);
    }
 end:
  va_end(args);
  if (tmperr) {
    g_propagate_error(error, tmperr);
    return FALSE;
  } else {
    return TRUE;
  }
}



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
static void start_location ( Loader *loader,
                             const gchar *element_name,
                             const gchar **atnames,
                             const gchar **atvalues,
                             GError **error );
static void end_location ( Loader *loader,
                           const gchar *element_name,
                           GError **error );
static void start_file ( Loader *loader,
                         const gchar *element_name,
                         const gchar **atnames,
                         const gchar **atvalues,
                         GError **error );
static void end_file ( Loader *loader,
                       const gchar *element_name,
                       GError **error );
static void start_namespace ( Loader *loader,
                              const gchar *element_name,
                              const gchar **atnames,
                              const gchar **atvalues,
                              GError **error );
static void end_namespace ( Loader *loader,
                            const gchar *element_name,
                            GError **error );
static void start_var ( Loader *loader,
                        const gchar *element_name,
                        const gchar **atnames,
                        const gchar **atvalues,
                        GError **error );
static void end_var ( Loader *loader,
                      const gchar *element_name,
                      GError **error );



static ElementFuncs ELEMENT_FUNCS[] =
  {
    { "root", start_root, end_root, NULL, },
    { "location", start_location, end_location, NULL, },
    { "file", start_file, end_file, NULL, },
    { "namespace", start_namespace, end_namespace, NULL, },
    { "var", start_var, end_var, NULL, },
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



/* loader_lookup_decl:
 */
static DokTree *loader_lookup_decl ( Loader *loader,
                                     DokTreeID id )
{
  DokTree *decl;
  decl = g_hash_table_lookup(loader->idmap,
                             GUINT_TO_POINTER(id));
  ASSERT(decl);
  return decl;
}



/* dok_tree_load:
 */
gboolean dok_tree_load ( DokTree *tree,
                         const gchar *fname )
{
  GMarkupParseContext *context;
  FILE *f;
  gchar buffer[1024];
  gint fno;
  GError *error = NULL;
  Loader *loader;

  CL_DEBUG("loading doktree file: '%s'", fname);

  loader = g_new0(Loader, 1);
  loader->tree = tree;
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

  return TRUE;
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
  /* loader->tree = dok_tree_root_new(); */
}



/* end_root:
 */
static void end_root ( Loader *loader,
                       const gchar *element_name,
                       GError **error )
{
  CL_TRACE("%s", element_name);
}



/* start_location:
 */
static void start_location ( Loader *loader,
                             const gchar *element_name,
                             const gchar **atnames,
                             const gchar **atvalues,
                             GError **error )
{
  guint fileid;
  gint lineno;
  gboolean isdef;
  DokTree *file, *loc;
  /* [FIXME] lineno is not guint */
  if (!collect_attrs(atnames, atvalues, error,
                     COLLECT_FLAG_UINT, "file", &fileid,
                     COLLECT_FLAG_UINT, "lineno", &lineno,
                     COLLECT_FLAG_BOOL, "isdef", &isdef,
                     0))
    CL_ERROR("[TODO]");
  file = g_hash_table_lookup(loader->idmap, GUINT_TO_POINTER(fileid));
  ASSERT(file);
  ASSERT(DOK_IS_TREE_FILE(file));
  loc = dok_tree_loc_new(file, lineno, isdef);
  dok_tree_decl_add_location2(loader->item, loc);
}



/* end_location:
 */
static void end_location ( Loader *loader,
                           const gchar *element_name,
                           GError **error )
{
}



/* start_file:
 */
static void start_file ( Loader *loader,
                         const gchar *element_name,
                         const gchar **atnames,
                         const gchar **atvalues,
                         GError **error )
{
  DokTree *tree;
  guint id;
  gchar *name;
  if (!collect_attrs(atnames, atvalues, error,
                     COLLECT_FLAG_STRING, "name", &name,
                     COLLECT_FLAG_UINT, "id", &id,
                     0))
    CL_ERROR("[TODO]");
  tree = dok_tree_file_new(loader->tree, name);
  g_hash_table_insert(loader->idmap,
                      GUINT_TO_POINTER(id),
                      tree);
  ASSERT(!loader->item);
  loader->item = tree;
}



/* end_file:
 */
static void end_file ( Loader *loader,
                       const gchar *element_name,
                       GError **error )
{
  loader->item = NULL;
}



/* start_namespace:
 */
static void start_namespace ( Loader *loader,
                              const gchar *element_name,
                              const gchar **atnames,
                              const gchar **atvalues,
                              GError **error )
{
  DokTree *tree, *context;
  guint id, context_id;
  gchar *name;
  if (!collect_attrs(atnames, atvalues, error,
                     COLLECT_FLAG_STRING, "name", &name,
                     COLLECT_FLAG_UINT, "id", &id,
                     COLLECT_FLAG_UINT, "context", &context_id,
                     0))
    CL_ERROR("[TODO]");
  if (context_id == 0) {
    context = NULL;
    ASSERT(!strcmp(name, ""));
    tree = dok_tree_root_get_root_namespace(loader->tree);
    ASSERT(tree);
  } else {
    context = dok_tree_root_get_decl_by_id(loader->tree, context_id);
    ASSERT(context);
    if ((tree = dok_tree_decl_get_namespace(context, name)))
      {
        CL_ERROR("[TODO]");
      }
    else
      {
        CL_ERROR("[TODO]");
      }
  }
  g_hash_table_insert(loader->idmap,
                      GUINT_TO_POINTER(id),
                      tree);
  ASSERT(!loader->item);
  loader->item = tree;
}



/* end_namespace:
 */
static void end_namespace ( Loader *loader,
                            const gchar *element_name,
                            GError **error )
{
  loader->item = NULL;
}



/* start_var:
 */
static void start_var ( Loader *loader,
                        const gchar *element_name,
                        const gchar **atnames,
                        const gchar **atvalues,
                        GError **error )
{
  DokTree *tree, *context;
  guint id, context_id;
  gchar *name;
  if (!collect_attrs(atnames, atvalues, error,
                     COLLECT_FLAG_STRING, "name", &name,
                     COLLECT_FLAG_UINT, "id", &id,
                     COLLECT_FLAG_UINT, "context", &context_id,
                     0))
    CL_ERROR("[TODO]");
  context = loader_lookup_decl(loader, context_id);
  if ((tree = dok_tree_decl_get_member(context, DOK_TYPE_TREE_VAR, name)))
    {
      CL_ERROR("[TODO] '%s'", name);
    }
  else
    {
      tree = dok_tree_var_new(context, name);
    }
  g_hash_table_insert(loader->idmap,
                      GUINT_TO_POINTER(id),
                      tree);
  ASSERT(!loader->item);
  loader->item = tree;
}



/* end_var:
 */
static void end_var ( Loader *loader,
                      const gchar *element_name,
                      GError **error )
{
  loader->item = NULL;
}
