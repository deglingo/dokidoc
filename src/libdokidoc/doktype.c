/* doktype.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/doktype.h"



/* TypeNode:
 */
typedef struct _TypeNode TypeNode;
struct _TypeNode
{
  DokType type;
  gchar *name;
  TypeNode *parent;
  DokTypeInfo info;
  DokTypeClass *type_class;
};



/* globals */
static GPtrArray *type_nodes = NULL;
static GHashTable *type_names = NULL;



/* _dok_type_init:
 */
void _dok_type_init ( void )
{
  type_nodes = g_ptr_array_new();
  g_ptr_array_add(type_nodes, NULL);
  type_names = g_hash_table_new(g_str_hash, g_str_equal);
}



/* type_lookup:
 */
static inline TypeNode *type_lookup ( DokType type )
{
  ASSERT(type > 0 && type < type_nodes->len);
  return type_nodes->pdata[type];
}



/* dok_type_register:
 */
DokType dok_type_register ( const gchar *name,
                            DokType parent,
                            DokTypeInfo *info )
{
  TypeNode *node, *parent_node;
  ASSERT(!dok_type_from_name(name));
  if (parent == 0) {
    parent_node = NULL;
    ASSERT(info->instance_size >= sizeof(DokTypeInstance));
    ASSERT(info->class_size >= sizeof(DokTypeClass));
  } else {
    parent_node = type_lookup(parent);
    ASSERT(info->instance_size >= parent_node->info.instance_size);
    ASSERT(info->class_size >= parent_node->info.class_size);
  }
  node = g_new0(TypeNode, 1);
  node->type = type_nodes->len;
  node->name = g_strdup(name);
  node->parent = parent_node;
  node->info = *info;
  /* class init */
  node->type_class = g_malloc0(node->info.class_size);
  node->type_class->dok_type = node->type;
  /* register */
  g_ptr_array_add(type_nodes, node);
  g_hash_table_insert(type_names, node->name, node);
  return node->type;
}



/* dok_type_name:
 */
const gchar *dok_type_name ( DokType type )
{
  TypeNode *node = type_lookup(type);
  return node->name;
}



/* dok_type_from_name:
 */
DokType dok_type_from_name ( const gchar *name )
{
  TypeNode *node = g_hash_table_lookup(type_names, name);
  return node ? node->type : 0;
}



/* dok_type_isa:
 */
gboolean dok_type_isa ( DokType type1,
                        DokType type2 )
{
  TypeNode *node = type_lookup(type1);
  while (node) {
    if (node->type == type2)
      return TRUE;
    node = node->parent;
  }
  return FALSE;
}



/* dok_type_instance_new:
 */
DokTypeInstance *dok_type_instance_new ( DokType type )
{
  TypeNode *node;
  DokTypeInstance *obj;
  node = type_lookup(type);
  obj = g_malloc0(node->info.instance_size);
  obj->dok_type_class = node->type_class;
  return obj;
}



/* dok_type_instance_cast:
 */
gpointer dok_type_instance_cast ( gpointer obj,
                                  DokType type )
{
  DokType obj_type;
  ASSERT(obj);
  ASSERT(dok_type_name(type));
  obj_type = DOK_TYPE_INSTANCE_GET_TYPE(obj);
  ASSERT(dok_type_isa(obj_type, type));
  return obj;
}



/* dok_type_instance_check:
 */
gboolean dok_type_instance_check ( gpointer obj,
                                   DokType type )
{
  return dok_type_isa(DOK_TYPE_INSTANCE_GET_TYPE(obj), type);
}
