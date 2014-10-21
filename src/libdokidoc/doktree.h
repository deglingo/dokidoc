/* doktree.h -
 */

#ifndef _DOKTREE_H_
#define _DOKTREE_H_

#include "libdokidoc/doktype.h"



typedef guint DokTreeID;
typedef struct _DokTree DokTree;
typedef struct _DokTreeClass DokTreeClass;
typedef struct _DokTreeChildren DokTreeChildren;
typedef struct _DokTreeLoc DokTreeLoc;
typedef struct _DokTreeItem DokTreeItem;
typedef struct _DokTreeFile DokTreeFile;
typedef struct _DokTreeDecl DokTreeDecl;
typedef struct _DokTreeRoot DokTreeRoot;
typedef struct _DokTreeNamespace DokTreeNamespace;
typedef struct _DokTreeVar DokTreeVar;



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



/* DokTreeLoc:
 */
struct _DokTreeLoc
{
  DokTree super;

  DokTree *file;
  gint lineno;
  gboolean isdef;
};



/* DokTreeRoot:
 */
struct _DokTreeRoot
{
  DokTree super;

  DokTreeID id_counter;
  GHashTable *decls_by_id;
  GHashTable *qfiles; /* map < GQuark qfile, DokTree *file > */
  DokTree *nsroot;
  GList *files;
};



/* DokTreeItem:
 */
struct _DokTreeItem
{
  DokTree super;

  DokTreeID id;
  DokTree *root;
  gchar *name;
};



/* DokTreeFile:
 */
struct _DokTreeFile
{
  DokTreeItem super;
};



/* DokTreeDecl:
 */
struct _DokTreeDecl
{
  DokTreeItem super;

  DokTree *context;
  GList *locations;
  GList *members;
};



/* DokTreeNamespace:
 */
struct _DokTreeNamespace
{
  DokTreeDecl super;
};



/* DokTreeVar:
 */
struct _DokTreeVar
{
  DokTreeDecl super;
};



/* DokTree */
#define DOK_TYPE_TREE (dok_tree_get_type())
#define DOK_TREE(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE, DokTree))

gchar *dok_tree_to_string ( DokTree *tree );

/* DokTreeLoc */
#define DOK_TYPE_TREE_LOC (dok_tree_loc_get_type())
#define DOK_TREE_LOC(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_LOC, DokTreeLoc))
#define DOK_IS_TREE_LOC(n) (DOK_TYPE_INSTANCE_CHECK((n), DOK_TYPE_TREE_LOC))

#define DOK_TREE_LOC_FILE(n) (DOK_TREE_LOC(n)->file)
#define DOK_TREE_LOC_LINENO(n) (DOK_TREE_LOC(n)->lineno)
#define DOK_TREE_LOC_ISDEF(n) (DOK_TREE_LOC(n)->isdef)

DokType dok_tree_loc_get_type ( void );

DokTree *dok_tree_loc_new ( DokTree *file,
                            gint lineno,
                            gboolean isdef );

/* DokTreeRoot */
#define DOK_TYPE_TREE_ROOT (dok_tree_root_get_type())
#define DOK_TREE_ROOT(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_ROOT, DokTreeRoot))
#define DOK_IS_TREE_ROOT(n) (DOK_TYPE_INSTANCE_CHECK((n), DOK_TYPE_TREE_ROOT))

DokType dok_tree_root_get_type ( void );
DokTree *dok_tree_root_new ( void );
DokTree *dok_tree_root_get_root_namespace ( DokTree *tree );
DokTree *dok_tree_root_get_decl_by_id ( DokTree *tree,
                                        DokTreeID id );
DokTree *dok_tree_root_lookup_qfile ( DokTree *tree,
                                      GQuark qfile );

/* DokTreeItem */
#define DOK_TYPE_TREE_ITEM (dok_tree_item_get_type())
#define DOK_TREE_ITEM(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_ITEM, DokTreeItem))
#define DOK_IS_TREE_ITEM(n) (DOK_TYPE_INSTANCE_CHECK((n), DOK_TYPE_TREE_ITEM))

#define DOK_TREE_ITEM_ROOT(n) (DOK_TREE_ITEM(n)->root)
#define DOK_TREE_ITEM_NAME(n) (DOK_TREE_ITEM(n)->name)
#define DOK_TREE_ITEM_ID(n) (DOK_TREE_ITEM(n)->id)

DokType dok_tree_item_get_type ( void );

/* DokTreeFile */
#define DOK_TYPE_TREE_FILE (dok_tree_file_get_type())
#define DOK_TREE_FILE(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_FILE, DokTreeFile))
#define DOK_IS_TREE_FILE(n) (DOK_TYPE_INSTANCE_CHECK((n), DOK_TYPE_TREE_FILE))

DokType dok_tree_file_get_type ( void );

DokTree *dok_tree_file_new ( DokTree *root,
                             const gchar *filename );

/* DokTreeDecl */
#define DOK_TYPE_TREE_DECL (dok_tree_decl_get_type())
#define DOK_TREE_DECL(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_DECL, DokTreeDecl))
#define DOK_IS_TREE_DECL(n) (DOK_TYPE_INSTANCE_CHECK((n), DOK_TYPE_TREE_DECL))

#define DOK_TREE_DECL_CONTEXT(n) (DOK_TREE_DECL(n)->context)
#define DOK_TREE_DECL_CONTEXT_ID(n) (DOK_TREE_DECL_CONTEXT(n) ? DOK_TREE_ITEM_ID(DOK_TREE_DECL_CONTEXT(n)) : 0)
/* [FIXME] */
#define DOK_TREE_DECL_FQNAME(n) (DOK_TREE_ITEM_NAME(n))


DokType dok_tree_decl_get_type ( void );

void dok_tree_decl_add_location ( DokTree *tree,
                                  DokLocation *loc,
                                  gboolean isdef );
/* [fixme] */
void dok_tree_decl_add_location2 ( DokTree *tree,
                                   DokTree *loc );
DokTree *dok_tree_decl_get_member ( DokTree *tree,
                                    DokType type,
                                    const gchar *name );

#define dok_tree_decl_get_namespace(tree, name) (dok_tree_decl_get_member((tree), DOK_TYPE_TREE_NAMESPACE, (name)))
#define dok_tree_decl_get_var(tree, name) (dok_tree_decl_get_member((tree), DOK_TYPE_TREE_VAR, (name)))

/* DokTreeNamespace */
#define DOK_TYPE_TREE_NAMESPACE (dok_tree_namespace_get_type())
#define DOK_TREE_NAMESPACE(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_NAMESPACE, DokTreeNamespace))
#define DOK_IS_TREE_NAMESPACE(n) (DOK_TYPE_INSTANCE_CHECK((n), DOK_TYPE_TREE_NAMESPACE))

DokType dok_tree_namespace_get_type ( void );

DokTree *dok_tree_namespace_new_root ( DokTree *root );
DokTree *dok_tree_namespace_new ( DokTree *context,
                                  const gchar *name );

/* DokTreeVar */
#define DOK_TYPE_TREE_VAR (dok_tree_var_get_type())
#define DOK_TREE_VAR(n) (DOK_TYPE_INSTANCE_CAST((n), DOK_TYPE_TREE_VAR, DokTreeVar))
#define DOK_IS_TREE_VAR(n) (DOK_TYPE_INSTANCE_CHECK((n), DOK_TYPE_TREE_VAR))

DokType dok_tree_var_get_type ( void );

DokTree *dok_tree_var_new ( DokTree *ctxt,
                            const gchar *name );



#endif
