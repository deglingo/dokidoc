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
GET_TYPE_FUNC(DokTreeNamespace, dok_tree_namespace, DOK_TYPE_TREE_DECL)



/* dok_tree_root_new:
 */
DokTree *dok_tree_root_new ( void )
{
  DokTree *tree = DOK_TREE(dok_type_instance_new(DOK_TYPE_TREE_ROOT));
  DokTreeRoot *root = DOK_TREE_ROOT(tree);
  root->decls = g_ptr_array_new();
  return tree;
}



/* dok_tree_decl_init:
 */
static void dok_tree_decl_init ( DokTree *decl,
                                 DokTree *context,
                                 const gchar *name )
{
  ASSERT(DOK_IS_TREE_DECL(decl));
  ASSERT((!context) || DOK_IS_TREE_DECL(context));

  DOK_TREE_DECL(decl)->context = context;
  DOK_TREE_DECL(decl)->name = g_strdup(name);
}



/* dok_tree_namespace_new:
 */
DokTree *dok_tree_namespace_new ( DokTree *context,
                                  const gchar *name )
{
  DokTree *tree = DOK_TREE(dok_type_instance_new(DOK_TYPE_TREE_NAMESPACE));
  dok_tree_decl_init(tree, context, name);
  return tree;
}
