/* doktree.h -
 */

#ifndef _DOKTREE_H_
#define _DOKTREE_H_

#include "libdokidoc/doktype.h"



typedef struct _DokTree DokTree;
typedef struct _DokTreeClass DokTreeClass;
typedef struct _DokTreeRoot DokTreeRoot;
typedef struct _DokTreeDecl DokTreeDecl;
typedef struct _DokTreeNamespace DokTreeNamespace;



/* DokTree:
 */
struct _DokTree
{
  DokTypeInstance super;
};



/* DokTreeClasss:
 */
struct _DokTreeClass
{
  DokTypeClass super;
};



/* DokTreeRoot:
 */
struct _DokTreeRoot
{
  DokTree super;

  GPtrArray *decls;
};



/* DokTreeDecl:
 */
struct _DokTreeDecl
{
  DokTree super;

  DokTree *context;
  gchar *name;
};



/* DokTreeNamespace:
 */
struct _DokTreeNamespace
{
  DokTreeDecl super;
};



/* DokTree */
#define DOK_TYPE_TREE (dok_tree_get_type())
#define DOK_TREE(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE, DokTree))

/* DokTreeRoot */
#define DOK_TYPE_TREE_ROOT (dok_tree_root_get_type())
#define DOK_TREE_ROOT(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_ROOT, DokTreeRoot))

DokType dok_tree_root_get_type ( void );
DokTree *dok_tree_root_new ( void );

/* DokTreeDecl */
#define DOK_TYPE_TREE_DECL (dok_tree_decl_get_type())
#define DOK_TREE_DECL(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_DECL, DokTreeDecl))
#define DOK_IS_TREE_DECL(n) (DOK_TYPE_INSTANCE_CHECK((n), DOK_TYPE_TREE_DECL))

/* DokTreeNamespace */
#define DOK_TYPE_TREE_NAMESPACE (dok_tree_namespace_get_type())
#define DOK_TREE_NAMESPACE(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TREE_TYPE_NAMESPACE, DokTreeNamespace))

DokTree *dok_tree_namespace_new ( DokTree *context,
                                  const gchar *name );



#endif
