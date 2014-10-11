/* doktree.c -
 */

#include "libdokidoc/doktree.h"
#include <clog.h>
#include <string.h>



#define ASSERT(expr) do {                           \
    if (!(expr)) {                                  \
      CL_ERROR("ASSERTION FAILED: `" #expr "'");    \
    }                                               \
  } while (0)



/* DumpContext:
 */
typedef struct _DumpContext
{
  gint depth;
}
  DumpContext;



DokTree *_dok_tree_new ( DokTreeType type )
{
  DokTree *tree = g_new0(DokTree, 1);
  tree->tree_type = type;
  return tree;
}



/* dok_tree_new:
 */
DokTree *dok_tree_new ( void )
{
  DokTree *tree = _dok_tree_new(DOK_TREE_ROOT);
  tree->t_root.decls = g_ptr_array_new();
  return tree;
}



static void _dok_tree_dump ( DokTree *tree,
                             xmlNodePtr ctxt )
{
  switch (tree->tree_type)
    {
    case DOK_TREE_ROOT:
      {
        xmlNodePtr node = xmlNewChild(ctxt, NULL, BAD_CAST "namespace", NULL);
        gint d;
        for (d = 0; d < tree->t_root.decls->len; d++)
          {
            DokTree *decl = tree->t_root.decls->pdata[d];
            _dok_tree_dump(decl, node);
          }
      }
      break;
    case DOK_TREE_VAR:
      {
        xmlNodePtr node = xmlNewChild(ctxt, NULL, BAD_CAST "var", NULL);
        xmlNodePtr type = xmlNewChild(node, NULL, BAD_CAST "type", NULL);
        xmlNewProp(node, BAD_CAST "name", BAD_CAST tree->t_var.name);
        _dok_tree_dump(tree->t_var.type, type);
      }
      break;
    case DOK_TREE_TYPE_NAME:
      {
        xmlNodePtr node = xmlNewChild(ctxt, NULL, BAD_CAST "type-name", NULL);
        xmlNewProp(node, BAD_CAST "name", BAD_CAST tree->t_type_name.name);
      }
      break;
    default:
      CL_ERROR("[TODO] %d", tree->tree_type);
    }
}



/* dok_tree_dump:
 */
xmlDocPtr dok_tree_dump ( DokTree *tree )
{
  xmlDocPtr doc;
  xmlNodePtr root;
  doc = xmlNewDoc((xmlChar *) "1.0");
  root = xmlNewNode(NULL, BAD_CAST "root");
  xmlDocSetRootElement(doc, root);
  _dok_tree_dump(tree, root);
  return doc;
}



/* dok_tree_new_var:
 */
DokTree *dok_tree_new_var ( const gchar *name,
                            DokTree *type )
{
  DokTree *var = _dok_tree_new(DOK_TREE_VAR);
  var->t_var.name = g_strdup(name);
  var->t_var.type = type;
  return var;
}



/* dok_tree_new_type_name:
 */
DokTree *dok_tree_new_type_name ( const gchar *type_name )
{
  DokTree *tree = _dok_tree_new(DOK_TREE_TYPE_NAME);
  tree->t_type_name.name = g_strdup(type_name);
  return tree;
}



/* dok_tree_add_decl:
 */
void dok_tree_add_decl ( DokTree *tree,
                         DokTree *decl )
{
  CL_TRACE("tree=%p, type=%d", tree, tree->tree_type);
  ASSERT(tree->tree_type == DOK_TREE_ROOT);
  g_ptr_array_add(tree->t_root.decls, decl);
}
