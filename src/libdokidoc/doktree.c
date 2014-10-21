/* doktree.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktree.h"

#include <string.h>



#define GET_TYPE_FUNC(cmlname, lowname, parent)             \
  DokType lowname##_get_type ( void ) {                     \
    static DokType type = 0;                                \
    if (!type) {                                            \
      DokTypeInfo info;                                     \
      info.class_size = sizeof(DokTreeClass /* ?? */);      \
      info.instance_size = sizeof(cmlname);                 \
      type = dok_type_register(#cmlname, parent, &info);    \
    }                                                       \
    return type;                                            \
  }



GET_TYPE_FUNC(DokTree, dok_tree, 0);
GET_TYPE_FUNC(DokTreeLoc, dok_tree_loc, DOK_TYPE_TREE)
GET_TYPE_FUNC(DokTreeRoot, dok_tree_root, DOK_TYPE_TREE)
GET_TYPE_FUNC(DokTreeItem, dok_tree_item, DOK_TYPE_TREE)
GET_TYPE_FUNC(DokTreeFile, dok_tree_file, DOK_TYPE_TREE_ITEM)
GET_TYPE_FUNC(DokTreeDecl, dok_tree_decl, DOK_TYPE_TREE_ITEM)
GET_TYPE_FUNC(DokTreeNamespace, dok_tree_namespace, DOK_TYPE_TREE_DECL)
GET_TYPE_FUNC(DokTreeVar, dok_tree_var, DOK_TYPE_TREE_DECL)



/* dok_tree_to_string:
 */
gchar *dok_tree_to_string ( DokTree *tree )
{
  return g_strdup_printf("<%s %p>", dok_type_name(DOK_TYPE_INSTANCE_GET_TYPE(tree)), tree);
}



/* dok_tree_loc_new:
 */
DokTree *dok_tree_loc_new ( DokTree *file,
                            gint lineno,
                            gboolean isdef )
{
  DokTree *tree = DOK_TREE(dok_type_instance_new(DOK_TYPE_TREE_LOC));
  ASSERT(DOK_IS_TREE_FILE(file));
  DOK_TREE_LOC(tree)->file = file;
  DOK_TREE_LOC(tree)->lineno = lineno;
  DOK_TREE_LOC(tree)->isdef = isdef;
  return tree;
}



/* dok_tree_root_new:
 */
DokTree *dok_tree_root_new ( void )
{
  DokTree *tree = DOK_TREE(dok_type_instance_new(DOK_TYPE_TREE_ROOT));
  DOK_TREE_ROOT(tree)->decls_by_id = g_hash_table_new(NULL, NULL);
  DOK_TREE_ROOT(tree)->qfiles = g_hash_table_new(NULL, NULL);
  DOK_TREE_ROOT(tree)->nsroot = dok_tree_namespace_new_root(tree);
  return tree;
}



/* dok_tree_root_get_root_namespace:
 */
DokTree *dok_tree_root_get_root_namespace ( DokTree *tree )
{
  return DOK_TREE_ROOT(tree)->nsroot;
}



/* dok_tree_root_get_decl_by_id:
 */
DokTree *dok_tree_root_get_decl_by_id ( DokTree *tree,
                                        DokTreeID id )
{
  return g_hash_table_lookup(DOK_TREE_ROOT(tree)->decls_by_id, GUINT_TO_POINTER(id));
}



/* dok_tree_root_lookup_qfile:
 */
DokTree *dok_tree_root_lookup_qfile ( DokTree *tree,
                                      GQuark qfile )
{
  DokTree *file;
  ASSERT(DOK_IS_TREE_ROOT(tree));
  file = g_hash_table_lookup(DOK_TREE_ROOT(tree)->qfiles,
                             GUINT_TO_POINTER(qfile));
  ASSERT(file);
  return file;
}



/* dok_tree_item_new:
 */
static DokTree *dok_tree_item_new ( DokType type,
                                    DokTree *root,
                                    const gchar *name )
{
  DokTree *item;
  ASSERT(dok_type_isa(type, DOK_TYPE_TREE_ITEM));
  item = DOK_TREE(dok_type_instance_new(type));
  DOK_TREE_ITEM(item)->name = g_strdup(name);
  DOK_TREE_ITEM(item)->root = root;
  DOK_TREE_ITEM(item)->id = ++(DOK_TREE_ROOT(root)->id_counter);
  DOK_TREE_ITEM(item)->name = g_strdup(name);
  g_hash_table_insert(DOK_TREE_ROOT(root)->decls_by_id,
                      GUINT_TO_POINTER(DOK_TREE_ITEM_ID(item)),
                      item);
  return item;
}



/* dok_tree_file_new:
 */
DokTree *dok_tree_file_new ( DokTree *root,
                             const gchar *filename )
{
  GQuark qfile = g_quark_from_string(filename);
  DokTree *tree = dok_tree_item_new(DOK_TYPE_TREE_FILE, root, filename);
  DOK_TREE_ROOT(root)->files = g_list_append(DOK_TREE_ROOT(root)->files, tree);
  ASSERT(!g_hash_table_lookup(DOK_TREE_ROOT(root)->qfiles, GUINT_TO_POINTER(qfile)));
  g_hash_table_insert(DOK_TREE_ROOT(root)->qfiles,
                      GUINT_TO_POINTER(qfile),
                      tree);
  return tree;
}



/* dok_tree_decl_new:
 */
static DokTree *dok_tree_decl_new ( DokType type,
                                    DokTree *context,
                                    const gchar *name )
{
  DokTree *decl;

  ASSERT(DOK_IS_TREE_DECL(context));

  decl = dok_tree_item_new(type, DOK_TREE_ITEM_ROOT(context), name);

  DOK_TREE_DECL(decl)->context = context;
  DOK_TREE_DECL(context)->members = 
    g_list_append(DOK_TREE_DECL(context)->members,
                  decl);
  return decl;
}



/* dok_tree_decl_add_location:
 */
void dok_tree_decl_add_location ( DokTree *decl,
                                  DokLocation *loc,
                                  gboolean isdef )
{
  DokTree *root, *file, *tree_loc;
  ASSERT(DOK_IS_TREE_DECL(decl));
  root = DOK_TREE_ITEM_ROOT(decl);
  file = dok_tree_root_lookup_qfile(root, loc->qfile);
  tree_loc = dok_tree_loc_new(file, loc->lineno, isdef);
  DOK_TREE_DECL(decl)->locations = 
    g_list_append(DOK_TREE_DECL(decl)->locations,
                  tree_loc);
}



/* dok_tree_decl_add_location2:
 */
void dok_tree_decl_add_location2 ( DokTree *decl,
                                   DokTree *loc )
{
  DOK_TREE_DECL(decl)->locations = 
    g_list_append(DOK_TREE_DECL(decl)->locations,
                  loc);
}



/* dok_tree_decl_get_member:
 */
DokTree *dok_tree_decl_get_member ( DokTree *tree,
                                    DokType type,
                                    const gchar *name )
{
  GList *l;
  ASSERT(DOK_IS_TREE_DECL(tree));
  ASSERT(type == 0 || dok_type_isa(type, DOK_TYPE_TREE_DECL));
  for (l = DOK_TREE_DECL(tree)->members; l; l = l->next)
    {
      DokTree *m = l->data;
      if ((type == 0 || dok_type_isa(DOK_TYPE_INSTANCE_GET_TYPE(m), type)) &&
          !strcmp(name, DOK_TREE_ITEM_NAME(m)))
        return m;
    }
  return NULL;
}



/* dok_tree_namespace_new_root:
 */
DokTree *dok_tree_namespace_new_root ( DokTree *root )
{
  DokTree *decl;
  ASSERT(DOK_IS_TREE_ROOT(root));
  decl = dok_tree_item_new(DOK_TYPE_TREE_NAMESPACE, root, "");
  return decl;
}



/* dok_tree_namespace_new:
 */
DokTree *dok_tree_namespace_new ( DokTree *context,
                                  const gchar *name )
{
  DokTree *tree = dok_tree_decl_new(DOK_TYPE_TREE_NAMESPACE, context, name);
  return tree;  
}



/* dok_tree_var_new:
 */
DokTree *dok_tree_var_new ( DokTree *context,
                            const gchar *name )
{
  DokTree *tree = dok_tree_decl_new(DOK_TYPE_TREE_VAR, context, name);
  return tree;
}
