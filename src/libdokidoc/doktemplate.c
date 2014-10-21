/* doktemplate.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktemplate.h"



/* Node:
 */
typedef struct _Node
{
  const gchar *type; /* [FIXME] enum */
  const gchar *fqname;
  const gchar *doc;
}
  Node;



/* dok_template_new:
 */
DokTemplate *dok_template_new ( const gchar *tmplfile )
{
  DokTemplate *tmpl;
  tmpl = g_new0(DokTemplate, 1);
  tmpl->path = g_strdup(tmplfile);
  tmpl->nodes = g_ptr_array_new();
  return tmpl;
}



/* dok_template_add_node:
 */
void dok_template_add_node ( DokTemplate *tmpl,
                             const gchar *type,
                             const gchar *fqname )
{
  Node *node = g_new0(Node, 1);
  /* [FIXME] check existing */
  node->type = g_strdup(type);
  node->fqname = g_strdup(fqname);
  node->doc = g_strdup("<EMPTY>");
  g_ptr_array_add(tmpl->nodes, node);
}



/* dok_template_dump:
 */
void dok_template_dump ( DokTemplate *tmpl,
                         FILE *f )
{
  guint n;
  fprintf(f, "# TEMPLATE: %s\n", tmpl->path);
  for (n = 0; n < tmpl->nodes->len; n++)
    {
      Node *node = tmpl->nodes->pdata[n];
      fprintf(f, "\n@@%s %s\n\n", node->type, node->fqname);
      fprintf(f, "%s\n", node->doc);
    }
}
