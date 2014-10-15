/* doktree.h -
 */

#ifndef _DOKTREE_H_
#define _DOKTREE_H_

#include "libdokidoc/doktype.h"



typedef guint DokTreeID;
typedef enum _DokTreeDeclType DokTreeDeclType;
typedef struct _DokTree DokTree;
typedef struct _DokTreeClass DokTreeClass;
typedef struct _DokTreeChildren DokTreeChildren;
typedef struct _DokTreeDecl DokTreeDecl;
typedef struct _DokTreeRoot DokTreeRoot;
typedef struct _DokTreeNamespace DokTreeNamespace;
typedef struct _DokTreeVar DokTreeVar;



/* DokTreeDeclType:
 */
enum _DokTreeDeclType
  {
    DOK_TREE_DECL_NAMESPACE,
    DOK_TREE_DECL_VAR,
  };



/* DokTree:
 */
struct _DokTree
{
  DokTypeInstance super;
};



/* DokTreeClass:
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

  DokTreeID id_counter;
  GPtrArray *decls;
};



/* DokTreeDecl:
 */
struct _DokTreeDecl
{
  DokTree super;

  DokTreeID id;
  DokTreeDeclType decl_type;
  DokTree *context;
  gchar *name;
};



/* DokTree */
#define DOK_TYPE_TREE (dok_tree_get_type())
#define DOK_TREE(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE, DokTree))

gchar *dok_tree_to_string ( DokTree *tree );

/* DokTreeRoot */
#define DOK_TYPE_TREE_ROOT (dok_tree_root_get_type())
#define DOK_TREE_ROOT(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_ROOT, DokTreeRoot))

DokType dok_tree_root_get_type ( void );
DokTree *dok_tree_root_new ( void );
DokTree *dok_tree_get_namespace ( DokTree *tree,
                                  DokTree *context,
                                  const gchar *name );
DokTree *dok_tree_get_var ( DokTree *tree,
                            DokTree *context,
                            const gchar *name );

/* DokTreeDecl */
#define DOK_TYPE_TREE_DECL (dok_tree_decl_get_type())
#define DOK_TREE_DECL(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_DECL, DokTreeDecl))
#define DOK_IS_TREE_DECL(n) (DOK_TYPE_INSTANCE_CHECK((n), DOK_TYPE_TREE_DECL))

DokType dok_tree_decl_get_type ( void );



#endif
