/* doktree.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktree.h"



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
GET_TYPE_FUNC(DokTreeRoot, dok_tree_root, DOK_TYPE_TREE)
GET_TYPE_FUNC(DokTreeDecl, dok_tree_decl, DOK_TYPE_TREE)



/* dok_tree_to_string:
 */
gchar *dok_tree_to_string ( DokTree *tree )
{
  return g_strdup_printf("<%s %p>", dok_type_name(DOK_TYPE_INSTANCE_GET_TYPE(tree)), tree);
}



/* dok_tree_root_new:
 */
DokTree *dok_tree_root_new ( void )
{
  DokTree *tree = DOK_TREE(dok_type_instance_new(DOK_TYPE_TREE_ROOT));
  DOK_TREE_ROOT(tree)->decls = g_ptr_array_new();
  DOK_TREE_ROOT(tree)->decls_id = g_hash_table_new(NULL, NULL);
  return tree;
}



/* dok_tree_get_decl_by_id:
 */
DokTree *dok_tree_get_decl_by_id ( DokTree *tree,
                                   DokTreeID id )
{
  return g_hash_table_lookup(DOK_TREE_ROOT(tree)->decls_id,
                             GUINT_TO_POINTER(id));
}



/* dok_tree_decl_new:
 */
static DokTree *dok_tree_decl_new ( DokTree *tree,
                                    DokTreeDeclType decl_type,
                                    DokTree *context,
                                    const gchar *name,
                                    DokLocation *loc )
{
  DokTree *decl;

  ASSERT((!context) || DOK_IS_TREE_DECL(context));

  decl = DOK_TREE(dok_type_instance_new(DOK_TYPE_TREE_DECL));
  DOK_TREE_DECL(decl)->id = ++(DOK_TREE_ROOT(tree)->id_counter);
  DOK_TREE_DECL(decl)->decl_type = decl_type;
  DOK_TREE_DECL(decl)->context = context;
  DOK_TREE_DECL(decl)->name = g_strdup(name);
  if (loc)
    DOK_TREE_DECL(decl)->loc = *loc;
  g_ptr_array_add(DOK_TREE_ROOT(tree)->decls, decl);
  g_hash_table_insert(DOK_TREE_ROOT(tree)->decls_id,
                      GUINT_TO_POINTER(DOK_TREE_DECL(decl)->id),
                      decl);
  return decl;
}



/* dok_tree_get_namespace:
 */
DokTree *dok_tree_get_namespace ( DokTree *tree,
                                  DokTree *context,
                                  const gchar *name )
{
  ASSERT(!context); /* [TODO] */
  /* [FIXME] find existing */
  DokTree *ns = dok_tree_decl_new(tree, DOK_TREE_DECL_NAMESPACE, context, name, NULL);
  return ns;
}



/* dok_tree_get_var:
 */
DokTree *dok_tree_get_var ( DokTree *tree,
                            DokTree *context,
                            const gchar *name,
                            DokLocation *loc )
{
  DokTree *var = dok_tree_decl_new(tree, DOK_TREE_DECL_VAR, context, name, loc);
  return var;
}
