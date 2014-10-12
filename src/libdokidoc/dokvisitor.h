/* dokvisitor.h -
 */

#ifndef _DOKVISITOR_H_
#define _DOKVISITOR_H_

#include "libdokidoc/dokbase.h"



typedef struct _DokVisitor DokVisitor;

typedef void (* DokVisitorFunc) ( DokVisitor *visitor,
                                  gpointer node );



/* DokVisitorClass:
 */
typedef struct _DokVisitorClass
{
  guint instance_size;

  gpointer (* get_node_type) ( gpointer node );

  gpointer (* get_node_type_parent) ( gpointer type );  

  DokVisitorFunc enter_default;

  DokVisitorFunc leave_default;

  GHashTable *table;
}
  DokVisitorClass;



/* DokVisitor:
 */
struct _DokVisitor
{
  DokVisitorClass *dok_visitor_class;
};



DokVisitorClass *dok_visitor_class_new ( void );
void dok_visitor_class_register_funcs ( DokVisitorClass *cls,
                                        gpointer node_type,
                                        DokVisitorFunc accept,
                                        DokVisitorFunc enter,
                                        DokVisitorFunc leave );

DokVisitor *dok_visitor_new ( DokVisitorClass *cls );
void dok_visitor_visit ( DokVisitor *visitor,
                         gpointer node );



#endif
