/* dokastvisitor.h -
 */

#ifndef _DOKASTVISITOR_H_
#define _DOKASTVISITOR_H_

#include "libdokidoc/dokbase.h"
#include "libdokidoc/dokvisitor.h"
#include "libdokidoc/dokast.h"



/* DokASTVisitor:
 */
typedef struct _DokASTVisitor
{
  DokVisitor dok_visitor;
}
  DokASTVisitor;



DokVisitorClass *dok_ast_visitor_class_new ( void );



#endif
