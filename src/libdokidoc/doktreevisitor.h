/* doktreevisitor.h -
 */

#ifndef _DOKTREEVISITOR_H_
#define _DOKTREEVISITOR_H_

#include "libdokidoc/dokbase.h"
#include "libdokidoc/dokvisitor.h"
#include "libdokidoc/doktree.h"



/* DokTreeVisitor:
 */
typedef struct _DokTreeVisitor
{
  DokVisitor dok_visitor;
}
  DokTreeVisitor;



DokVisitorClass *dok_tree_visitor_class_new ( void );



#endif
