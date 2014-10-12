/* doktree.h -
 */

#ifndef _DOKTREE_H_
#define _DOKTREE_H_

#include "libdokidoc/dokbase.h"



typedef enum _DokTreeType DokTreeType;
typedef union _DokTree DokTree;
typedef struct _DokTreeRoot DokTreeRoot;
typedef struct _DokTreeVar DokTreeVar;
typedef struct _DokTreeTypeName DokTreeTypeName;

#define DOK_TREE_HEADER \
  DokTreeType tree_type



/* DokTreeType:
 */
enum _DokTreeType
  {
    DOK_TREE_ROOT,
    DOK_TREE_VAR,
    DOK_TREE_TYPE_NAME,
  };



/* DokTreeRoot:
 */
struct _DokTreeRoot
{
  DOK_TREE_HEADER;

  GPtrArray *decls;
};



/* DokTreeVar:
 */
struct _DokTreeVar
{
  DOK_TREE_HEADER;

  gchar *name;
  DokTree *type;
};



/* DokTreeTypeName:
 */
struct _DokTreeTypeName
{
  DOK_TREE_HEADER;
  gchar *name;
};



/* DokTree:
 */
union _DokTree
{
  DokTreeType tree_type;
  DokTreeRoot t_root;
  DokTreeVar t_var;
  DokTreeTypeName t_type_name;
};



DokTree *dok_tree_new ( void );
xmlDocPtr dok_tree_dump ( DokTree *tree );
DokTree *dok_tree_new_var ( const gchar *name,
                            DokTree *type );
DokTree *dok_tree_new_type_name ( const gchar *type_name );
void dok_tree_add_decl ( DokTree *tree,
                         DokTree *decl );



#endif
